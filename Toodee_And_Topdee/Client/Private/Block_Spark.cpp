#include "Block_Spark.h"
#include "GameInstance.h"


CBlock_Spark::CBlock_Spark(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Spark::CBlock_Spark(const CBlock_Spark& Prototype)
    : CBlock{ Prototype }
{
}

HRESULT CBlock_Spark::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Spark::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Wall");

    return S_OK;
}

void CBlock_Spark::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Spark::Update(_float fTimeDelta)
{
}

void CBlock_Spark::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Spark::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Spark::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Spark"),
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

void CBlock_Spark::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CBlock_Spark::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Spark* CBlock_Spark::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Spark* pInstance = new CBlock_Spark(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Spark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Spark::Clone(void* pArg)
{
	CBlock_Spark* pInstance = new CBlock_Spark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Spark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Spark::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
