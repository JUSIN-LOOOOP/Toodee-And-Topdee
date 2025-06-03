#include "Player_Topdee.h"
#include "GameInstance.h"
#include "PlayerState.h"

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
	m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].iMaxAnimCount = 1;


	return S_OK;
}

HRESULT CPlayer_Topdee::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;
	
	if (FAILED(Ready_Observers()))
		return E_FAIL;

	if (FAILED(Ready_Outline()))
		return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	m_bCanClear = false;
	m_fTurnDownTime = 0.f;
	m_fTurnDownDelay = 0.04f;
	m_vPotalPosition = { 0.f, 0.f, 0.f };
	m_vNextMovePosition = { -1.f, 0.f, -1.f };

	m_pTransformCom->Scaling(12.f, 12.f, 0.f); 
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	
	ComputeTileCenter();

	return S_OK;
}

void CPlayer_Topdee::Priority_Update(_float fTimeDelta)
{
	Check_Dimension();


	if (GetKeyState('2') & 0x8000)
		Notify(EVENT::ENTER_PORTAL);
}

void CPlayer_Topdee::Update(_float fTimeDelta)
{
	if (m_eCurrentState != PLAYERSTATE::CLEAR)
	{
		if (m_eCurrentState != PLAYERSTATE::STOP)
		{
			_uint iInputData = KeyInput();

			m_pCurrentState->HandleInput(this, iInputData, fTimeDelta);

			ComputeTextureDirection(iInputData);
			Change_MoveDir(iInputData);
			
			if (m_eCurrentState == PLAYERSTATE::IDLE)
				MoveToTileCenter(fTimeDelta);

			m_pGameInstance->Check_Collision(m_pColliderCom);

			Check_CollisionState();

			if (m_bInAction)
			{
				m_bInAction = false;
			}
		}
		else
		{
			if (!m_bIsTurnDown)
				TurnDownOnStop(fTimeDelta);
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
	m_iCurrentAnimCount = m_pCurrentState->GetAnimCount() + ( ENUM_CLASS(m_eCurrentMoveDir) * m_pCurrentState->GetMaxAnimCount());


	if (m_eCurrentState == PLAYERSTATE::STOP)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
	else
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer_Topdee::Render()
{
	if (FAILED(m_pColliderCom->Render()))
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

void CPlayer_Topdee::Move(_float fTimeDelta)
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
	m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);

	m_ePrevMoveDir = m_eCurrentMoveDir;
	m_bIsTurnDown = false;
}

void CPlayer_Topdee::Clear()
{
	m_bMoveToPotal = false;

	Change_TextureDir(TEXTUREDIRECTION::RIGHT);
	m_eCurrentMoveDir = MOVEDIRECTION::DOWN;

	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	m_vPotalStartPosition = { m_vPotalPosition.x + m_fPotalDistance, m_vPotalPosition.y, m_vPotalPosition.z };

	_float3 vSpeed = m_vPotalStartPosition - vPosition;

	m_fClearSpeedPerSec = D3DXVec3Length(&vSpeed);
}

void CPlayer_Topdee::onReport(REPORT eReport)
{
	if (eReport == REPORT::REPORT_CANCLEAR)
		m_bCanClear = true;
}

_float3 CPlayer_Topdee::ComputeTileOutlinePosition()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	const _float TILE_SIZE = 2.f;

	_int iCountX = static_cast<_int>(floorf(vPosition.x / TILE_SIZE));
	_int iCountZ = static_cast<_int>(floorf(vPosition.z / TILE_SIZE));

	_float fCenterX = {};
	_float fCenterZ = {};

	fCenterX = iCountX * TILE_SIZE + (TILE_SIZE * 0.5);
	fCenterZ = iCountZ * TILE_SIZE + (TILE_SIZE * 0.5);

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


	_float3 vOutlinePosition = {fCenterX + fDistanceX, vPosition.y, fCenterZ + fDistanceZ};

	return vOutlinePosition;
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

void CPlayer_Topdee::ComputeTileCenter()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	const _float TILE_SIZE = 2.f;

	_int iCountX = static_cast<_int>(floorf(vPosition.x / TILE_SIZE));
	_int iCountZ = static_cast<_int>(floorf(vPosition.z / TILE_SIZE));

	_float fCenterX = {};
	_float fCenterZ = {};

	fCenterX = iCountX * TILE_SIZE + (TILE_SIZE * 0.5);
	fCenterZ = iCountZ * TILE_SIZE + (TILE_SIZE * 0.5);

	m_vCurrentTileCenter = { fCenterX, vPosition.y, fCenterZ };
}

void CPlayer_Topdee::MoveToTileCenter(_float fTimeDelta)
{
	m_pTransformCom->Approach(m_vCurrentTileCenter, fTimeDelta, 10.f);	
}

HRESULT CPlayer_Topdee::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform*/
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 10.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

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

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPlayer_Topdee::Ready_States()
{
	for (_uint i = 0; i < ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END); i++)
	{
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

HRESULT CPlayer_Topdee::Ready_Observers()
{
	m_pGameInstance->Subscribe_Observer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Observer_ClearTrigger"), this);

	return S_OK;
}

HRESULT CPlayer_Topdee::Ready_Outline()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Effect"),
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
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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

void CPlayer_Topdee::Check_CollisionState()
{
	if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
	{
		_float fDist = {};
		_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		COLLIDER_DIR eCollider_Dir = m_pColliderCom->DetectCollisionDirection(&fDist);
		
		switch(eCollider_Dir)
		{
		case COLLIDER_DIR::LEFT:
			vPosition.x -= fDist;
			break;
		case COLLIDER_DIR::RIGHT:
			vPosition.x += fDist;
			break;
		case COLLIDER_DIR::TOP:
			vPosition.y += fDist;
			break;
		case COLLIDER_DIR::BOTTOM:
			vPosition.y -= fDist;
			break;
		case COLLIDER_DIR::FRONT:
			vPosition.z -= fDist;
			break;
		case COLLIDER_DIR::BACK:
			vPosition.z += fDist;
			break;
		}
		m_pTransformCom->Set_State(STATE::POSITION, vPosition);
		m_vNextMovePosition = vPosition;
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
}
