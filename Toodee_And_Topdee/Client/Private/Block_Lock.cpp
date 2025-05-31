#include "Block_Lock.h"
#include "GameInstance.h"


CBlock_Lock::CBlock_Lock(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Lock::CBlock_Lock(const CBlock_Lock& Prototype)
    : CBlock{ Prototype }
{
}

HRESULT CBlock_Lock::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Lock::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Lock");

    return S_OK;
}

void CBlock_Lock::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Lock::Update(_float fTimeDelta)
{

}

void CBlock_Lock::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Lock::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Lock::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Lock"),
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

void CBlock_Lock::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CBlock_Lock::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Lock* CBlock_Lock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Lock* pInstance = new CBlock_Lock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Lock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Lock::Clone(void* pArg)
{
	CBlock_Lock* pInstance = new CBlock_Lock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Lock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Lock::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);


}
