#include "Player_Topdee.h"
#include "GameInstance.h"

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
	m_iMaxAnimCount[ENUM_CLASS(PS_IDLE)] = 0;
	m_iMaxAnimCount[ENUM_CLASS(PS_MOVE)] = 8;
	m_iMaxAnimCount[ENUM_CLASS(PS_CLEAR)] = 17;

	m_eCurrentDir = DIR_R;
	m_ePreDir = m_eCurrentDir;

	m_eMoveDir = MD_DOWN;

	m_fAnimDelay = 0.05f;

	//포탈과의 거리
	m_fPotalDistance = 5.f;

	m_bInput = false;

	return S_OK;
}

HRESULT CPlayer_Topdee::Initialize(void* pArg)
{
	m_eMoveDir = MD_DOWN;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(70.f, 70.f, 0.f); 
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	
	return S_OK;
}

void CPlayer_Topdee::Priority_Update(_float fTimeDelta)
{
	//Test
	if (!m_bClear)
	{
		_float3 vTarget = { 0.f, 0.f, 0.f }; // 포탈 위치

		__super::MoveToPotal(vTarget, _float3(0.f, 1.f, 0.f), fTimeDelta);

	}

	_float3 scale = m_pTransformCom->Get_Scaled();
	int a;
}

void CPlayer_Topdee::Update(_float fTimeDelta)
{
	m_iInputState = 0;

	if (m_pGameInstance->Key_Down('Z'))
		m_bInput = m_bInput ? false : true;

	if(m_bInput)
	{
		if (m_pGameInstance->Key_Pressing(VK_RIGHT))
		{
			m_eCurrentState = PS_MOVE;
			m_eCurrentDir = DIR_R;
			m_iInputState |= ID_RIGHT;
			m_pTransformCom->Go_Right(fTimeDelta);
		}

		if (m_pGameInstance->Key_Pressing(VK_LEFT))
		{
			m_eCurrentState = PS_MOVE;
			m_eCurrentDir = DIR_L;
			m_iInputState |= ID_LEFT;
			m_pTransformCom->Go_Right(fTimeDelta);
		}

		if (m_pGameInstance->Key_Pressing(VK_UP))
		{
			m_eCurrentState = PS_MOVE;
			m_iInputState |= ID_UP;
			m_pTransformCom->Go_Up(fTimeDelta);
		}

		if (m_pGameInstance->Key_Pressing(VK_DOWN))
		{
			m_eCurrentState = PS_MOVE;
			m_iInputState |= ID_DOWN;
			m_pTransformCom->Go_Down(fTimeDelta);
		}
	}




	/* 입력 없었으면 마지막 입력을 기준으로 방향 결정 */
	if (m_iInputState == 0 && m_eCurrentState != PS_CLEAR)
	{
		m_eCurrentState = PS_IDLE;
		m_iInputState = m_iOldInputState;
	}
	else
		m_iOldInputState = m_iInputState;

	__super::Change_State();
	__super::Change_Dir();
	m_eMoveDir = Check_Move_Direction(m_iInputState);
}

void CPlayer_Topdee::Late_Update(_float fTimeDelta)
{
	/* 애니메이션 카운트 딜레이 */

	//클리어(포탈 입장) 애니메이션
	if (m_eCurrentState == PS_CLEAR)
	{
		if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
		{
			if (m_iCurrentAnimCount < m_iMaxAnimCount[ENUM_CLASS(PS_CLEAR)] - 1)
			{
				m_iCurrentAnimCount = m_iCurrentAnimCount++;
				m_fAnimTime = 0.f;
			}
		}
		else
			m_fAnimTime += fTimeDelta;
	}
	else if (m_eCurrentState != PS_IDLE)
	{
		/* MoveDir에 따른 AnimCount 초기값 설정 */
		if (m_iCurrentAnimCount == 0)
			m_iCurrentAnimCount = (m_iMaxAnimCount[ENUM_CLASS(m_eCurrentState)] * ENUM_CLASS(m_eMoveDir));

		if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
		{
			m_iCurrentAnimCount = ((m_iCurrentAnimCount + 1) % m_iMaxAnimCount[ENUM_CLASS(m_eCurrentState)])
				+ (m_iMaxAnimCount[ENUM_CLASS(m_eCurrentState)] * ENUM_CLASS(m_eMoveDir));
			m_fAnimTime = 0.f;
		}
		else
			m_fAnimTime += fTimeDelta;
	}
	else
		m_iCurrentAnimCount = ENUM_CLASS(m_eMoveDir);


	/* Toddee 시점에서 블렌딩 */
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

HRESULT CPlayer_Topdee::Render()
{
	m_pTransformCom->Bind_Matrix();

	m_pTextureComs[ENUM_CLASS(m_eCurrentState)]->Bind_Texture(m_iCurrentAnimCount);

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();
	return S_OK;
}

CPlayer_Topdee::MOVE_DIRECTION CPlayer_Topdee::Check_Move_Direction(_uint iInputState)
{
	if (((iInputState & ID_UP) && (iInputState & ID_RIGHT)) || (iInputState & ID_UP) && (iInputState & ID_LEFT))
		return MD_DIAGONAL_UP;

	if (((iInputState & ID_DOWN) && (iInputState & ID_RIGHT)) || (iInputState & ID_DOWN) && (iInputState & ID_LEFT))
		return MD_DIAGONAL_DOWN;

	if (iInputState & ID_UP)
		return MD_UP;

	if (iInputState & ID_DOWN)
		return MD_DOWN;

	if ((iInputState & ID_LEFT) || (iInputState & ID_RIGHT))
		return MD_TRANSVERSE;

	return MD_DOWN;
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
		TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_IDLE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Move"),
		TEXT("Com_Move_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_MOVE)]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Clear"),
		TEXT("Com_Clear_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_CLEAR)]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Topdee::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

//	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//if (m_eCurrentState == PS_STOP)
	//{
	//	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//}
	//else
	//	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	
	return S_OK;
}

HRESULT CPlayer_Topdee::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
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
