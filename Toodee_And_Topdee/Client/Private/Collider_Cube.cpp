#include "Collider_Cube.h"
#include "GameInstance.h"


CCollider_Cube::CCollider_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CCollider_Cube::CCollider_Cube(const CCollider_Cube& Prototype)
    : CBlock{ Prototype }
	, m_pColliderCom{ Prototype.m_pColliderCom }
{
	Safe_AddRef(m_pColliderCom);
}

HRESULT CCollider_Cube::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCollider_Cube::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Wall");

    return S_OK;
}

void CCollider_Cube::Priority_Update(_float fTimeDelta)
{
	
}

void CCollider_Cube::Update(_float fTimeDelta)
{

	/*_bool newkey = GetKeyState(VK_RIGHT) & 0x8000;
	if (!key[0] && newkey)
		tmpIdx++;
	key[0] = newkey;

	newkey = GetKeyState(VK_LEFT) & 0x8000;
	if (!key[1] && newkey)
		tmpIdx--;
	key[1] = newkey;*/

}

void CCollider_Cube::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCollider_Cube::Render()
{
	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;
	// __super::Render();

	return S_OK;
}

HRESULT CCollider_Cube::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Wall"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC		ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.bIsFixed = false ;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CCollider_Cube::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CCollider_Cube::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CCollider_Cube* CCollider_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Cube* pInstance = new CCollider_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollider_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCollider_Cube::Clone(void* pArg)
{
	CCollider_Cube* pInstance = new CCollider_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCollider_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Cube::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
