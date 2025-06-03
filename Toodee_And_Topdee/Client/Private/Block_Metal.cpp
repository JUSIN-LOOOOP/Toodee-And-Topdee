#include "Block_Metal.h"
#include "GameInstance.h"


CBlock_Metal::CBlock_Metal(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Metal::CBlock_Metal(const CBlock_Metal& Prototype)
    : CBlock{ Prototype }
{
}

HRESULT CBlock_Metal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Metal::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Wall");

    return S_OK;
}

void CBlock_Metal::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Metal::Update(_float fTimeDelta)
{

}

void CBlock_Metal::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Metal::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Metal::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Metal"),
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

void CBlock_Metal::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CBlock_Metal::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Metal* CBlock_Metal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Metal* pInstance = new CBlock_Metal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Metal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Metal::Clone(void* pArg)
{
	CBlock_Metal* pInstance = new CBlock_Metal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Metal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Metal::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
