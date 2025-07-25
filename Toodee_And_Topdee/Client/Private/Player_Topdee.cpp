﻿#include "Player_Topdee.h"
#include "GameInstance.h"
#include "PlayerState.h"
#include "InteractionBlock.h"
#include "ColliderMap_Object.h"
#include "Key.h"
#include "Block.h"
#include "Block_Spark.h"
#include "Level_Loading.h"

CPlayer_Topdee::CPlayer_Topdee(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayer { pGraphic_Device }
{
}

CPlayer_Topdee::CPlayer_Topdee(const CPlayer_Topdee& Prototype)
	: CPlayer { Prototype}
{
}

HRESULT CPlayer_Topdee::Initialize_Prototype()
{
	m_bMoveInAction = false;
	//��Ż���� �Ÿ�
	m_fPotalDistance = 5.f;
	
	m_eActivateDimension = DIMENSION::TOPDEE;

	m_eCurrentTextureDir = TEXTUREDIRECTION::RIGHT;

	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::IDLE)].eState = PLAYERSTATE::IDLE;
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::IDLE)].iMaxAnimCount = 1;

	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::MOVE)].eState = PLAYERSTATE::MOVE;
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::MOVE)].iMaxAnimCount = 8;

	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::ACTION)].eState = PLAYERSTATE::ACTION;
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::ACTION)].iMaxAnimCount = 1;

	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::STOP)].eState = PLAYERSTATE::STOP;
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::STOP)].iMaxAnimCount = 5;

	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::CLEAR)].eState = PLAYERSTATE::CLEAR;
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::CLEAR)].iMaxAnimCount = 17;

	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].eState = PLAYERSTATE::DEAD;
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].iMaxAnimCount = 5;


	return S_OK;
}

HRESULT CPlayer_Topdee::Initialize(void* pArg)
{
	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();


	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;
	
	if (FAILED(Ready_SubscribeEvent(m_iPlayLevel)))
		return E_FAIL;

	if (FAILED(Ready_Outline(m_iPlayLevel)))
		return E_FAIL;

	if (nullptr == pArg)
	{
		m_vPotalPosition = { 0.f, 0.f, 0.f };
		m_pTransformCom->Set_State(STATE::POSITION, _float3(1.f, 0.5f, 1.f));
	}
	else
	{
		BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

		_float3 vPosition = pDesc->vPos;
		vPosition.y += 1.f;
		m_pTransformCom->Set_State(STATE::POSITION, vPosition);
	}

	m_bCanClear = false;
	m_fTurnDownTime = 0.f;
	m_fTurnDownDelay = 0.04f;
	m_vPotalPosition = { 0.f, 0.f, 0.f };

	m_fMaxDot = -2.f;

	m_pTransformCom->Scaling(12.f, 12.f, 0.f); 
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	
	name = TEXT("Topdee");
	ComputeTileCenter();

	PLAYERSPOSITION_EVENT event;
	event.eWho = DIMENSION::TOPDEE;
	event.pTransformTopdee = m_pTransformCom;
	m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::PLAYERS_POSITION, event);

	m_ePreDimension = m_pGameInstance->Get_CurrentDimension();
	if (m_ePreDimension == DIMENSION::TOODEE)
	{
		m_fCurAngle = m_fToodeeAngle; m_fCurLightY = m_fToodeeLightDirY;
	}
	else
	{
		m_fCurAngle = m_fTopdeeAngle; m_fCurLightY = m_fTopdeeLightDirY;
	}

	return S_OK;
}

void CPlayer_Topdee::Priority_Update(_float fTimeDelta)
{
	if (m_iPlayLevel == ENUM_CLASS(LEVEL::LEVEL_DIALOGUE))
		return;

	Check_Dimension();

	m_pActionCheckColliderCom->Collision_Off();

}

void CPlayer_Topdee::Update(_float fTimeDelta)
{
	if (m_iPlayLevel == ENUM_CLASS(LEVEL::LEVEL_DIALOGUE))
		return;

	if (m_eCurrentState != PLAYERSTATE::CLEAR)
	{
		if (m_eCurrentState != PLAYERSTATE::STOP && m_eCurrentState != PLAYERSTATE::DEAD)
		{
			_uint iInputData = KeyInput();

			m_pCurrentState->HandleInput(this, iInputData, fTimeDelta);

			ComputeTextureDirection(iInputData);
			Change_MoveDir(iInputData);
			
			if (m_eCurrentState == PLAYERSTATE::IDLE)
				MoveToTileCenter(fTimeDelta);

			m_pGameInstance->Check_Collision(m_pColliderCom);

			Check_Collision();

			if (m_bInAction)
			{
				MoveToTileCenter(fTimeDelta);

				if (m_pAttachBlock)
					Check_DetachCollisionState();
				else
					Check_AttachCollisionState();

				Interaction();
			}
		}
		else
		{
			if (!m_bIsTurnDown)
				TurnDownOnStop(fTimeDelta);

			m_bInAction = false;
		}

		m_pCurrentState->Update(this, fTimeDelta);

	}
	else
	{
		m_pCurrentState->Update(this, fTimeDelta);

		if (!m_bMoveToPotal)
		{
			if (m_pTransformCom->Approach(m_vPotalStartPosition, fTimeDelta, m_fClearSpeedPerSec))
			{
				m_bMoveToPotal = true;
				m_bClearAnimStart = true;
			}
		}

		if (m_bClearAnimStart)
		{
			if (m_pTransformCom->Spiral(m_vPotalPosition, _float3(0.f, 1.f, 0.f), 480.f, m_fPotalDistance, fTimeDelta))
			{
				m_bClear = true;
			}
		}
	}

}

void CPlayer_Topdee::Late_Update(_float fTimeDelta)
{
	if (m_eCurrentState == PLAYERSTATE::DEAD)
		int a = 10;

	m_iCurrentAnimCount = m_pCurrentState->GetAnimCount() + ( ENUM_CLASS(m_eCurrentMoveDir) * m_pCurrentState->GetMaxAnimCount());


	if (m_eCurrentState == PLAYERSTATE::STOP)
	{
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
		__super::Compute_CamDistance(m_pTransformCom);
	}
	else if (m_eCurrentState == PLAYERSTATE::CLEAR)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	else
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer_Topdee::Render()
{
	Render_Shadow();

	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;

	if (FAILED(m_pActionCheckColliderCom->Render()))
		return E_FAIL;

	m_pTransformCom->Bind_Matrix();

	if (m_eCurrentState == PLAYERSTATE::STOP)
		m_pTextureComs[ENUM_CLASS(m_eCurrentState)]->Bind_Texture(ENUM_CLASS(m_eCurrentMoveDir));		//Stop OutLine Render
	else
		m_pTextureComs[ENUM_CLASS(m_eCurrentState)]->Bind_Texture(m_iCurrentAnimCount);

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();
	return S_OK;
}

HRESULT CPlayer_Topdee::Return_PrevState()
{
	if (m_pPrevState == nullptr)
		return E_FAIL;

	m_pColliderCom->Collision_On();

	if (m_pCurrentState)
	{
		m_pCurrentState->Exit(this);
		Safe_Release(m_pCurrentState);
	}

	m_pCurrentState = m_pPrevState;
	m_eCurrentState = m_pPrevState->GetTextureKey();

	Safe_Release(m_pPrevState);

	Safe_AddRef(m_pCurrentState);

	if (m_pPrevState)
		m_pPrevState = nullptr;

	m_eCurrentMoveDir = m_ePrevMoveDir;

	return S_OK;
}

void CPlayer_Topdee::Idle()
{
	ComputeTileCenter();
}

void CPlayer_Topdee::Move(_uint iInputData, _float fTimeDelta)
{
	if (m_pColliderCom->OnCollisionExit())
		return;

	m_vNextMovePosition = ComputeTileOutlinePosition();
	 
	m_pTransformCom->MoveUntilInRange(m_vNextMovePosition, fTimeDelta, 0.1f);

}

void CPlayer_Topdee::Action()
{
	m_bInAction = true;
}

void CPlayer_Topdee::Stop()
{
	m_pColliderCom->Collision_Off();
	//m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);

	m_ePrevMoveDir = m_eCurrentMoveDir;
	m_bIsTurnDown = false;

	m_bInAction = false;
}

void CPlayer_Topdee::Clear(_float3 vPortalPosition)
{
	m_bMoveToPotal = false;

	Change_TextureDir(TEXTUREDIRECTION::RIGHT);
	m_eCurrentMoveDir = MOVEDIRECTION::DOWN;

	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	m_vPotalStartPosition = { vPortalPosition.x + m_fPotalDistance, vPortalPosition.y, vPortalPosition.z };

	m_vPotalPosition = vPortalPosition;

	_float3 vSpeed = m_vPotalStartPosition - vPosition;

	m_fClearSpeedPerSec = D3DXVec3Length(&vSpeed);

	m_pColliderCom->Collision_Off();
}

void CPlayer_Topdee::Dead()
{
	m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
	m_pGameInstance->PlayAudio(TEXT("Restart.wav"), CHANNELID::SOUND_EFFECT, 0.5f);

	LEVELCHANGE_EVENT Event;
	Event.iChangeLevel = m_iPlayLevel;
	Event.iCurrentLevel = m_iPlayLevel;
	m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
}

void CPlayer_Topdee::Interaction()
{

	if (m_pGameInstance->Key_Up('Z'))
	{
		if (m_pAttachBlock && true == m_bCanDetach)
		{

			m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
			m_pGameInstance->PlayAudio(TEXT("BlockDetach.wav"), CHANNELID::SOUND_PLAYER, 0.5f);

			m_pAttachBlock->Request_Change_State(BLOCKSTATE::DETACH);
			m_pAttachBlock->Detach(m_vDetachPosition, 20.f);
			m_pAttachBlock = nullptr;
			m_bIsAttach = false;
		}
		else
		{	
			if (m_pFocuseBlock)
			{
				if(m_pFocuseBlock->IsStop())
				{
					m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
					m_pGameInstance->PlayAudio(TEXT("BlockAttach.wav"), CHANNELID::SOUND_PLAYER, 0.5f);

					m_pFocuseBlock->Request_Change_State(BLOCKSTATE::ATTACH);
					m_pFocuseBlock->Attach(m_pTransformCom, 20.f);
					m_pAttachBlock = m_pFocuseBlock;
					m_bIsAttach = true;
				}
				m_pFocuseBlock = nullptr;
			}
		}
		m_bInAction = false;
	}
}

_float3 CPlayer_Topdee::ComputeTileOutlinePosition()
{
	_float3 vCenter = ComputeTileCenter();

	const _float TILE_SIZE = 2.f;

	_float fDistanceX = {};
	_float fDistanceZ = {};

	switch (m_eCurrentMoveDir)
	{
	case MOVEDIRECTION::UP:
		fDistanceZ = TILE_SIZE;
		break;
	case MOVEDIRECTION::DOWN:
		fDistanceZ = -TILE_SIZE;
		break;
	case MOVEDIRECTION::TRANSVERSE:
		fDistanceX = TILE_SIZE;
		break;
	case MOVEDIRECTION::DIAGONAL_DOWN:
		fDistanceX = TILE_SIZE;
		fDistanceZ = -TILE_SIZE;
		break;
	case MOVEDIRECTION::DIAGONAL_UP:
		fDistanceX = TILE_SIZE;
		fDistanceZ = TILE_SIZE;
		break;
	}

	if (m_eCurrentTextureDir == TEXTUREDIRECTION::LEFT)
	{
		fDistanceX *= -1.f;
	}

	_float3 vOutlinePosition = { vCenter.x + fDistanceX, vCenter.y, vCenter.z + fDistanceZ};

	return vOutlinePosition;
}

_bool CPlayer_Topdee::IsAttachSparkBlock( )
{
	if (m_bIsAttach && reinterpret_cast<CGameObject*>(m_pAttachBlock)->CompareName(TEXT("Interaction_Block_Spark")))
		return true;

	return false;
}

void CPlayer_Topdee::Check_Dimension()
{
	if (m_eActivateDimension == m_pGameInstance->Get_CurrentDimension())
	{
		m_pColliderCom->Collision_On();
		m_bCanActive = true;
	}
	else
	{
		m_pColliderCom->Collision_Off();
		m_bCanActive = false;
		
		if (m_pAttachBlock)
		{
			Check_DetachCollisionState();
			
			if (true == m_bCanDetach)
			{
				m_pAttachBlock->Request_Change_State(BLOCKSTATE::DETACH);
				m_pAttachBlock->Detach(m_vDetachPosition, 20.f);
				m_pAttachBlock = nullptr;
				m_bIsAttach = false;
			}
		}
	}
}

_uint CPlayer_Topdee::KeyInput()
{
	_uint iInputData = 0;

	if (m_pGameInstance->Key_Pressing(VK_LEFT))
		iInputData |= ENUM_CLASS(KEYINPUT::KEY_LEFT);

	if (m_pGameInstance->Key_Pressing(VK_RIGHT))
		iInputData |= ENUM_CLASS(KEYINPUT::KEY_RIGHT);

	if (m_pGameInstance->Key_Pressing(VK_UP))
		iInputData |= ENUM_CLASS(KEYINPUT::KEY_UP);

	if (m_pGameInstance->Key_Pressing(VK_DOWN))
		iInputData |= ENUM_CLASS(KEYINPUT::KEY_DOWN);

	if (m_pGameInstance->Key_Pressing('Z'))
		iInputData |= ENUM_CLASS(KEYINPUT::KEY_Z);

	if (m_pGameInstance->Key_Down('X'))
		iInputData |= ENUM_CLASS(KEYINPUT::KEY_X);

	return iInputData;
}

void CPlayer_Topdee::Change_MoveDir(_uint iInputData)
{
	MOVEDIRECTION eNewMoveDir = ComputeMoveDirection(iInputData);
	
	if (m_eCurrentMoveDir == eNewMoveDir)
		return;

	m_eCurrentMoveDir = eNewMoveDir;
}

MOVEDIRECTION CPlayer_Topdee::ComputeMoveDirection(_uint iInputData)
{
	if(  ((iInputData & ENUM_CLASS(KEYINPUT::KEY_UP)) && (iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT))) ||
		((iInputData & ENUM_CLASS(KEYINPUT::KEY_UP)) && (iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT)))   )
		return MOVEDIRECTION::DIAGONAL_UP;

	if(   ((iInputData & ENUM_CLASS(KEYINPUT::KEY_DOWN)) && (iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT))) ||
		((iInputData & ENUM_CLASS(KEYINPUT::KEY_DOWN)) && (iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT))))
		return MOVEDIRECTION::DIAGONAL_DOWN;

	if (iInputData & ENUM_CLASS(KEYINPUT::KEY_UP))
		return MOVEDIRECTION::UP;

	if (iInputData & ENUM_CLASS(KEYINPUT::KEY_DOWN))
		return MOVEDIRECTION::DOWN;

	if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT)) || (iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT)))
		return MOVEDIRECTION::TRANSVERSE;

	return m_eCurrentMoveDir;
}

_float3 CPlayer_Topdee::ComputeTileCenter()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	const _float TILE_SIZE = 2.f;

	_int iCountX = static_cast<_int>(floorf(vPosition.x / TILE_SIZE));
	_int iCountZ = static_cast<_int>(floorf(vPosition.z / TILE_SIZE));

	_float fCenterX = {};
	_float fCenterZ = {};

	fCenterX = static_cast<_float>(iCountX) * TILE_SIZE + static_cast<_float>(TILE_SIZE * 0.5);
	fCenterZ = static_cast<_float>(iCountZ) * TILE_SIZE + static_cast<_float>(TILE_SIZE * 0.5);

	m_vCurrentTileCenter = { fCenterX, vPosition.y, fCenterZ };

	return m_vCurrentTileCenter;
}

void CPlayer_Topdee::MoveToTileCenter(_float fTimeDelta)
{
	m_pTransformCom->Approach(m_vCurrentTileCenter, fTimeDelta, 10.f);	
}

void CPlayer_Topdee::Check_AttachCollisionState()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	vPosition = { vPosition.x, vPosition.y, vPosition.z };
	m_pActionCheckTransformCom->Set_State(STATE::POSITION, vPosition);

	m_fMaxDot = -2.f;

	m_pActionCheckColliderCom->Collision_On();

	m_pGameInstance->Check_Collision(m_pActionCheckColliderCom);

	if (m_pActionCheckColliderCom->OnCollisionStay() || m_pActionCheckColliderCom->OnCollisionEnter())
	{
		vector<CGameObject*>* Overlaps = { nullptr };
		m_pActionCheckColliderCom->GetOverlapAll(Overlaps);

		_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		_float3 vLook = Compute_Look();

		for(auto iter : *Overlaps)
		{
			if ((iter->Get_Name().find(TEXT("Interaction")) != string::npos))
			{
				CInteractionBlock* pBlock = dynamic_cast<CInteractionBlock*>(iter);

				_float fDot = pBlock->ComputeDirDotLook(vPosition, vLook);

				if (fDot > m_fMaxDot)
				{
					m_fMaxDot = fDot;
					m_pFocuseBlock = pBlock;
				}
			}

		}
	}
}

_float3 CPlayer_Topdee::Compute_Look()
{
	_float3 vLook = { 0.f, 0.f, 0.f };

	switch (m_eCurrentMoveDir)
	{
	case MOVEDIRECTION::DOWN:
		vLook.z = -1.f;
		break;
	case MOVEDIRECTION::DIAGONAL_DOWN:
		vLook.x = 0.5f;
		vLook.z = -0.5f;
		break;
	case MOVEDIRECTION::TRANSVERSE:
		vLook.x = 1.f;
		break;
	case MOVEDIRECTION::DIAGONAL_UP:
		vLook.x = 0.5f;
		vLook.z = 0.5f;
		break;
	case MOVEDIRECTION::UP:
		vLook.z = 1.f;
		break;
	}

	if (m_eCurrentTextureDir == TEXTUREDIRECTION::LEFT)
		vLook.x *= -1.f;

	return vLook;
}

void CPlayer_Topdee::Check_DetachCollisionState()
{
	// Check 전 초기화

	for (_uint i = 0; i < 4; i++)
	{
		m_fDotByDir[i] = -2.f;
	}

	m_fMaxDot = -2.f;

	// LEFT, UP, RIGHT, DOWN 방향 Check 용

	_float3 Dir[4] = { {-1.f, 0.f, 0.f},
						{ 0.f, 0.f, 1.f},
						{ 1.f, 0.f, 0.f },
						{ 0.f, 0.f, -1.f} };

	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	m_pActionCheckTransformCom->Set_State(STATE::POSITION, vPosition);

	m_pActionCheckColliderCom->Collision_On();

	m_pGameInstance->Check_Collision(m_pActionCheckColliderCom);

	if (m_pActionCheckColliderCom->OnCollisionStay() || m_pActionCheckColliderCom->OnCollisionEnter())
	{
		vector<CGameObject*>* Overlaps = { nullptr };
		m_pActionCheckColliderCom->GetOverlapAll(Overlaps);

		_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		_float3 vLook = Compute_Look();

		for (auto iter : *Overlaps)
		{
			if ((iter->Get_Name().find(TEXT("Block")) != string::npos))
			{
				CBlock* pBlock = dynamic_cast<CBlock*>(iter);

				_float fDot = pBlock->ComputeDirDotLook(vPosition, vLook);

				if (fDot > m_fMaxDot)
				{
					m_fMaxDot = fDot;
				}

				for (_uint i = 0; i < 4; i++)
				{
					_float fDotByDir = pBlock->ComputeDirDotLook(vPosition, Dir[i]);

					if (fDotByDir > m_fDotByDir[i])
					{
						m_fDotByDir[i] = fDotByDir;
					}
				}
			}
			else if (iter->Get_Name().find(TEXT("Wall")) != string::npos)
			{
				CColliderMap_Object* pWall = dynamic_cast<CColliderMap_Object*>(iter);

				_float fResult = static_cast<_float>(pWall->IsOverlappingWall(vPosition, vLook));

				if (fResult > m_fMaxDot)
				{
					m_fMaxDot = fResult;
				}

				for (_uint i = 0; i < 4; i++)
				{
					_float fDotByDir = static_cast<_float>(pWall->IsOverlappingWall(vPosition, Dir[i]));

					if (fDotByDir > m_fDotByDir[i])
					{
						m_fDotByDir[i] = fDotByDir;
					}
				}
			}
		}
	}

	if (m_fMaxDot <= 0.9f) // 오차 범위 감안
	{
		m_vDetachPosition = ComputeTileOutlinePosition();
		m_bCanDetach = true;
		return;
	}
	else
	{
		for (_uint i = 0; i < ENUM_CLASS(DETACHDIRECTION::DETACHDIRECTION_END); i++)
		{
			if (m_fDotByDir[i] <= 0.9f) // 오차 범위 감안
			{
				m_vDetachPosition = Compute_Detach_Position(static_cast<DETACHDIRECTION>(i));
				m_bCanDetach = true;
				return;
			}
		}
	}

	m_bCanDetach = false;
}

_float3 CPlayer_Topdee::Compute_Detach_Position(DETACHDIRECTION eDir)
{
	_float3 vCenter = ComputeTileCenter();

	const _float TILE_SIZE = 2.f;

	_float fDistanceX = 0.f;
	_float fDistanceZ = 0.f;

	switch (eDir)
	{
	case DETACHDIRECTION::LEFT:
		fDistanceX = -2.f;
		break;
	case DETACHDIRECTION::UP:
		fDistanceZ = 2.f;
		break;
	case DETACHDIRECTION::RIGHT:
		fDistanceX = 2.f;
		break;
	case DETACHDIRECTION::DOWN:
		fDistanceZ = -2.f;
		break;
	}

	_float3 vDetachPosition = { vCenter.x + fDistanceX, vCenter.y + 1.f, vCenter.z + fDistanceZ};
	return vDetachPosition;
}

HRESULT CPlayer_Topdee::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_DiffuseCube"),
		TEXT("Com_VIBuffer2"), reinterpret_cast<CComponent**>(&m_VIBufferCom_Diffuse))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Shader_Player"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
		return E_FAIL;

#pragma region Transform
	/* For.Com_Transform*/
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		ActionCheckTransformDesc{};
	ActionCheckTransformDesc.fSpeedPerSec = 0.f;
	ActionCheckTransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_AttachCheckTransform"), reinterpret_cast<CComponent**>(&m_pActionCheckTransformCom), &ActionCheckTransformDesc)))
		return E_FAIL;

#pragma endregion 

#pragma region Collider
	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ActionCheckColliderDesc{};
	ActionCheckColliderDesc.pOwner = this;
	ActionCheckColliderDesc.pTransform = m_pActionCheckTransformCom;
	ActionCheckColliderDesc.vColliderScale = _float3(3.f, 3.f, 3.f);
	ActionCheckColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ActionCheckColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_AttachCheckCollider"), reinterpret_cast<CComponent**>(&m_pActionCheckColliderCom), &ActionCheckColliderDesc)))
		return E_FAIL;

#pragma endregion

#pragma region Topdee Texture

	/* For.Com_Texture*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Idle"),
		TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::IDLE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Move"),
		TEXT("Com_Move_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::MOVE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Action"),
		TEXT("Com_Action_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::ACTION)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Stop"),
		TEXT("Com_Stop_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::STOP)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Clear"),
		TEXT("Com_Clear_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::CLEAR)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Dead"),
		TEXT("Com_Dead_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::DEAD)]))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CPlayer_Topdee::Ready_States()
{
	for (_uint i = 0; i < ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END); i++)
	{
		if (i == ENUM_CLASS(PLAYERSTATE::SWIM))
			continue;

		PLAYERSTATE eState = m_tStateInitDesc[i].eState;

		if (FAILED(__super::Add_State(eState, &m_tStateInitDesc[i])))
			return E_FAIL;
	}

	m_pCurrentState = Find_State(PLAYERSTATE::IDLE);

	if (!m_pCurrentState)
		return E_FAIL;

	Safe_AddRef(m_pCurrentState);

	return S_OK;
}

HRESULT CPlayer_Topdee::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<CANCLEAREVENT>(m_iPlayLevel, EVENT_KEY::CAN_CLEAR, [this](const CANCLEAREVENT& Event) {
		this->ClearReady(Event);
		});

	return S_OK;
}

HRESULT CPlayer_Topdee::Ready_Outline(_uint iPlayLevel)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iPlayLevel, TEXT("Layer_Effect"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_TileOutline"), this)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CPlayer_Topdee::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_eCurrentState == PLAYERSTATE::STOP)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		__super::Begin_Shader();
	}
	else if (m_eCurrentState == PLAYERSTATE::CLEAR)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	else
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	
	return S_OK;
}

HRESULT CPlayer_Topdee::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_eCurrentState == PLAYERSTATE::STOP)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		__super::End_Shader();
	}

	else if (m_eCurrentState == PLAYERSTATE::CLEAR)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	else
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CPlayer_Topdee::TurnDownOnStop(_float fTimeDelta)
{
	if (ENUM_CLASS(m_eCurrentMoveDir) > ENUM_CLASS(MOVEDIRECTION::DOWN))
	{
		if (m_fTurnDownTime + fTimeDelta > m_fTurnDownDelay)
		{
			m_eCurrentMoveDir = static_cast<MOVEDIRECTION>(ENUM_CLASS(m_eCurrentMoveDir) - 1);
			m_fTurnDownTime = 0.f;
		}
		else
			m_fTurnDownTime += fTimeDelta;
	}

	m_bIsTurnDown = m_eCurrentMoveDir == MOVEDIRECTION::DOWN;
}

void CPlayer_Topdee::Check_Collision()
{
	if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
	{
		vector<CGameObject*>* Overlaps = { nullptr };
		if (false == m_pColliderCom->GetOverlapAll(Overlaps))
			return;
		{
			for (auto iter : *Overlaps)
			{
				Check_Collision_Portal(iter);
				Check_Collision_Dead(iter);
				Check_Collision_InteractionBlock(iter);
				Check_Collision_Key(iter);
			}
		}

		Check_Collision_PlayerState();
	}
	else
	{
		if (m_bEnterPortal)
		{ 
			m_bEnterPortal = false;
			EXITPORTALEVENT Event;
			Event.pPlayer = this;
			m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::EXIT_PORTAL, Event);
		}
	}

}

void CPlayer_Topdee::Check_Collision_PlayerState()
{
	_float3 temp;
	if (m_pColliderCom->GetCollisionsOffset(&temp))
	{
		_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		vPosition.x = vPosition.x + temp.x;
		vPosition.z = vPosition.z + temp.z;

		m_pTransformCom->Set_State(STATE::POSITION, vPosition);
		m_vNextMovePosition = vPosition;
	}

}

void CPlayer_Topdee::Check_Collision_Dead(CGameObject* pGameObject)
{
	if (pGameObject->Get_Name().find(TEXT("Enemy")) != string::npos)
	{
		m_eCurrentMoveDir = MOVEDIRECTION::DOWN;

		m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::DEAD);
	}
}

void CPlayer_Topdee::Check_Collision_InteractionBlock(CGameObject* pGameObject)
{
	if(pGameObject->Get_Name().find(TEXT("Interaction")) != string::npos)
	{
		CInteractionBlock* pBlock = dynamic_cast<CInteractionBlock*>(pGameObject);
		if (false == pBlock->IsPush() && false == pBlock->IsFall())
		{
			pBlock->Request_Change_State(BLOCKSTATE::PUSH);
			m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
			m_pGameInstance->PlayAudio(TEXT("BlockPush.wav"), CHANNELID::SOUND_EFFECT, 0.5f);

			pBlock->Push(m_eCurrentMoveDir, m_eCurrentTextureDir, 8.f);
		}
	}
}

void CPlayer_Topdee::Check_Collision_Portal(CGameObject* pGameObject)
{
	if (pGameObject->Get_Name().find(TEXT("Portal")) != string::npos)
	{
		m_bEnterPortal = true;
		ENTERPORTALEVENT Event;
		Event.pPlayer = this;
		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::ENTER_PORTAL, Event);
	}
}

void CPlayer_Topdee::Check_Collision_Key(CGameObject* pGameObject)
{
	if (pGameObject->Get_Name().find(TEXT("Key")) != string::npos)
	{
		CKey* pKey = dynamic_cast<CKey*>(pGameObject);
		pKey->Get_Key();
		GETKEYEVENT Event;
		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::GET_KEY, Event);
	}
}

CPlayer_Topdee* CPlayer_Topdee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer_Topdee* pInstance = new CPlayer_Topdee(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CPlayer_Topdee"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Topdee::Clone(void* pArg)
{
	CPlayer_Topdee* pInstance = new CPlayer_Topdee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CPlayer_Topdee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Topdee::Free()
{
	__super::Free();
	
	Safe_Release(m_pActionCheckColliderCom);
	Safe_Release(m_pActionCheckTransformCom);
}
