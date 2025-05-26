#include "Block.h"
#include "GameInstance.h"


CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBlock::CBlock(const CBlock& Prototype)
	: CGameObject{ Prototype }

{
}

HRESULT CBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlock::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->fPos);
	m_pTransformCom->Scaling(pDesc->fScale.x, pDesc->fScale.y, 2);

	return S_OK;
}

void CBlock::Priority_Update(_float fTimeDelta)
{
}

void CBlock::Update(_float fTimeDelta)
{
}

void CBlock::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CBlock::Render()
{
	m_pTransformCom->Bind_Matrix();
	m_pGraphic_Device->SetTexture(0, nullptr);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/*if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;*/
	
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CBlock::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_MAP), TEXT("Prototype_Component_Texture_Cube"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}

void CBlock::SetUp_RenderState()
{
}

void CBlock::Reset_RenderState()
{
}

CBlock* CBlock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock* pInstance = new CBlock(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock::Clone(void* pArg)
{
	CBlock* pInstance = new CBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
