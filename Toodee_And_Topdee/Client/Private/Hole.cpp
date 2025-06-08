#include "Hole.h"
#include "GameInstance.h"


CHole::CHole(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CHole::CHole(const CHole& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CHole::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHole::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	m_pTransformCom->Go_Up(0.1f);
	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	name = TEXT("Hole");

    return S_OK;
}

void CHole::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
		m_pColliderCom->Collision_Off();
	else
		m_pColliderCom->Collision_On();
}

void CHole::Update(_float fTimeDelta)
{
	if (m_bDead)
		m_pColliderCom->Collision_Off();
}

void CHole::Late_Update(_float fTimeDelta)
{
	if(!m_bDead)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CHole::Render()
{
	m_pColliderCom->Render();
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIdx)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CHole::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Hole"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CHole* CHole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHole* pInstance = new CHole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CHole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHole::Clone(void* pArg)
{
	CHole* pInstance = new CHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CHole"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHole::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
