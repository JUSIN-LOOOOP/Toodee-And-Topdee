#include "Block_Metal.h"
#include "GameInstance.h"
#include "BlockState.h"


CBlock_Metal::CBlock_Metal(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CInteractionBlock { pGraphic_Device }
{
}

CBlock_Metal::CBlock_Metal(const CBlock_Metal& Prototype)
    : CInteractionBlock{ Prototype }
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

	if (FAILED(__super::Ready_State()))
		return E_FAIL;

	__super::SetUp_BlockInfo(pArg);

	m_fMaxFallHeight = 1.7f;

	name = TEXT("Interaction_Block_Metal");

    return S_OK;
}

void CBlock_Metal::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta); // Block 상태 초기화
}

void CBlock_Metal::Update(_float fTimeDelta)
{
	if (m_bFallinHole)
		return;

	m_pCurrentState->Update(this, fTimeDelta);

	m_pGameInstance->Check_Collision(m_pColliderCom);

	__super::CheckCollisionTopdeeState();
}

void CBlock_Metal::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (m_bFallinHole)
		return;

	if (m_eCurrentState == BLOCKSTATE::PUSH)
		__super::Update_PushState(this);
}

HRESULT CBlock_Metal::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBlock_Metal::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_DiffuseCube"),
		TEXT("Com_VIBuffer2"), reinterpret_cast<CComponent**>(&m_VIBufferCom_Diffuse))))
		return E_FAIL;

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
}
