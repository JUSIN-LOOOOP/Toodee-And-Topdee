#include "Block_Fall.h"
#include "GameInstance.h"


CBlock_Fall::CBlock_Fall(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Fall::CBlock_Fall(const CBlock_Fall& Prototype)
    : CBlock{ Prototype }
{
}

HRESULT CBlock_Fall::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Fall::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Wall");

    return S_OK;
}

void CBlock_Fall::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Fall::Update(_float fTimeDelta)
{

}

void CBlock_Fall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Fall::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Fall::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Fall"),
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

void CBlock_Fall::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CBlock_Fall::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Fall* CBlock_Fall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Fall* pInstance = new CBlock_Fall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Fall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Fall::Clone(void* pArg)
{
	CBlock_Fall* pInstance = new CBlock_Fall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Fall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Fall::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);


}
