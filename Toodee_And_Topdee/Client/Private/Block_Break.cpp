#include "Block_Break.h"
#include "GameInstance.h"


CBlock_Break::CBlock_Break(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Break::CBlock_Break(const CBlock_Break& Prototype)
    : CBlock { Prototype }
{
}

HRESULT CBlock_Break::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Break::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Wall_Break");

    return S_OK;
}

void CBlock_Break::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Break::Update(_float fTimeDelta)
{

}

void CBlock_Break::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Break::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Break::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Break"),
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
	ColliderDesc.vColliderScale = _float3(1.8f, 1.8f, 1.8f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CBlock_Break::SetUp_RenderState()
{
	__super::SetUp_RenderState();

}

void CBlock_Break::Reset_RenderState()
{
	__super::Reset_RenderState();
}


CBlock_Break* CBlock_Break::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Break* pInstance = new CBlock_Break(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Break"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Break::Clone(void* pArg)
{
	CBlock_Break* pInstance = new CBlock_Break(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Break"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Break::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);


}
