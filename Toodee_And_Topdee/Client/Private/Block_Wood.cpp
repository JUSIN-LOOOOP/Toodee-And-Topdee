#include "Block_Wood.h"
#include "GameInstance.h"


CBlock_Wood::CBlock_Wood(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Wood::CBlock_Wood(const CBlock_Wood& Prototype)
    : CBlock{ Prototype }
{
}

HRESULT CBlock_Wood::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Wood::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Wood");

    return S_OK;
}

void CBlock_Wood::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Wood::Update(_float fTimeDelta)
{

}

void CBlock_Wood::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Wood::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Wood::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Wood"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

void CBlock_Wood::SetUp_RenderState()
{
	__super::SetUp_RenderState();

}

void CBlock_Wood::Reset_RenderState()
{
	__super::Reset_RenderState();

}

CBlock_Wood* CBlock_Wood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Wood* pInstance = new CBlock_Wood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Wood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Wood::Clone(void* pArg)
{
	CBlock_Wood* pInstance = new CBlock_Wood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Wood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Wood::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);


}
