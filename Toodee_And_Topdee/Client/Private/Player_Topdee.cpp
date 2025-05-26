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
	
	m_bCanClear = false;
	m_fTurnDownTime = 0.f;
	m_fTurnDownDelay = 0.04f;
	m_vPotalPosition = { 0.f, 0.f, 0.f };

	m_pTransformCom->Scaling(16.f, 16.f, 0.f); 
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	
	return S_OK;
}

void CPlayer_Topdee::Priority_Update(_float fTimeDelta)
{
	Check_Dimension();
}

void CPlayer_Topdee::Update(_float fTimeDelta)
{
	if (!m_bCanClear)
	{
		if (m_eCurrentState != PLAYERSTATE::STOP)
		{
			_uint iInputData = KeyInput();

			m_pCurrentState->HandleInput(this, iInputData, fTimeDelta);

			ComputeTextureDirection(iInputData);
			Change_MoveDir(iInputData);

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
		if (!m_bMoveToPotal)
		{
			m_bMoveToPotal = m_pTransformCom->Move_To(m_vPotalStartPosition, fTimeDelta, m_fClearSpeedPerSec, 0.f);

			if (m_bMoveToPotal)
				m_bClearAnimStart = true;
		}

		if (m_bClearAnimStart)
		{
			m_pCurrentState->Update(this, fTimeDelta);

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

void CPlayer_Topdee::Move(_float fTimeDelta)
{
	switch (m_eCurrentMoveDir)
	{
	case MOVEDIRECTION::UP:
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	case MOVEDIRECTION::DOWN:
		m_pTransformCom->Go_Down(fTimeDelta);
		break;
	case MOVEDIRECTION::TRANSVERSE:
		m_pTransformCom->Go_Right(fTimeDelta);
		break;
	case MOVEDIRECTION::DIAGONAL_DOWN:
		m_pTransformCom->Go_Right(fTimeDelta);
		m_pTransformCom->Go_Down(fTimeDelta);
		break;
	case MOVEDIRECTION::DIAGONAL_UP:
		m_pTransformCom->Go_Right(fTimeDelta);
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	}
}

void CPlayer_Topdee::Action()
{
	m_bInAction = true;
}

void CPlayer_Topdee::Stop()
{
	m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);

	m_ePrevMoveDir = m_eCurrentMoveDir;
	m_bIsTurnDown = false;

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

HRESULT CPlayer_Topdee::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform*/
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Idle"),
		TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::IDLE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Move"),
		TEXT("Com_Move_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::MOVE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Action"),
		TEXT("Com_Action_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::ACTION)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Stop"),
		TEXT("Com_Stop_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::STOP)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Clear"),
		TEXT("Com_Clear_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::CLEAR)]))))
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
