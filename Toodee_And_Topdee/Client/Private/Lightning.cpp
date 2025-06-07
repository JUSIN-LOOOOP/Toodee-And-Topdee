#include "Lightning.h"
#include "GameInstance.h"

CLightning::CLightning(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPoolableObject{ pGraphic_Device }
{
}

CLightning::CLightning(const CLightning& Prototype)
	:CPoolableObject{ Prototype }
{
}

HRESULT CLightning::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLightning::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.2f, 1.f, 0.4f);
	//m_pTransformCom->Scaling(50.f, 10.f, 5.f);

	name = TEXT("Rain");

	return S_OK;
}

void CLightning::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CLightning::Update(_float fTimeDelta)
{
	if (!m_bActive) return;


}

void CLightning::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CLightning::Render()
{
	if (!m_bActive) return S_OK;

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	return S_OK;
}

HRESULT CLightning::Initialize_Pool(void* pArg)
{


	return S_OK;
}

HRESULT CLightning::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Lightning"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	return S_OK;

	/* For.Com_Collision */
	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 3.0f, 1.0f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		MSG_BOX(TEXT("Failed to Add_Component : Com_Collision"));
		return E_FAIL;
	}
}

void CLightning::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CLightning::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

CLightning* CLightning::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLightning* pInstance = new CLightning(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLightning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLightning::Clone(void* pArg)
{
	CLightning* pInstance = new CLightning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLightning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightning::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
