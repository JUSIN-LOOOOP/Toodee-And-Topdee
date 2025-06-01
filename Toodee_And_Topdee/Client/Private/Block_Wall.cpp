#include "Block_Wall.h"
#include "GameInstance.h"


CBlock_Wall::CBlock_Wall(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Wall::CBlock_Wall(const CBlock_Wall& Prototype)
    : CBlock{ Prototype }
{
}

HRESULT CBlock_Wall::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Wall::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Block_Wall");

    return S_OK;
}

void CBlock_Wall::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Wall::Update(_float fTimeDelta)
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

void CBlock_Wall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Wall::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Wall::Ready_Components()
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

void CBlock_Wall::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CBlock_Wall::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Wall* CBlock_Wall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Wall* pInstance = new CBlock_Wall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Wall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Wall::Clone(void* pArg)
{
	CBlock_Wall* pInstance = new CBlock_Wall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Wall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Wall::Free()
{
	__super::Free();

}
