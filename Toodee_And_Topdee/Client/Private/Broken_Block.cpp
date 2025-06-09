#include "Broken_Block.h"
#include "GameInstance.h"

CBroken_Blocks::CBroken_Blocks(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPoolableObject{ pGraphic_Device }
{
}

CBroken_Blocks::CBroken_Blocks(const CBroken_Blocks& Prototype)
	:CPoolableObject{ Prototype }
{
}

HRESULT CBroken_Blocks::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBroken_Blocks::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	name = TEXT("Broken_Block");

	return S_OK;
}

void CBroken_Blocks::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CBroken_Blocks::Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CBroken_Blocks::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

HRESULT CBroken_Blocks::Render()
{
	if (!m_bActive) return S_OK;
	return S_OK;
}

HRESULT CBroken_Blocks::Initialize_Pool(void* pArg)
{
	return S_OK;

}

HRESULT CBroken_Blocks::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Broken_Block"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.f, 1.0f, 1.0f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		MSG_BOX(TEXT("Failed to Add_Component : Com_Collider"));
		return E_FAIL;
	}

	return S_OK;

}

void CBroken_Blocks::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBroken_Blocks::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

CBroken_Blocks* CBroken_Blocks::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBroken_Blocks* pInstance = new CBroken_Blocks(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBroken_Blocks"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CBroken_Blocks::Clone(void* pArg)
{
	CBroken_Blocks* pInstance = new CBroken_Blocks(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBroken_Blocks"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBroken_Blocks::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
