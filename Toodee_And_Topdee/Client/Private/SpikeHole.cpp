#include "SpikeHole.h"
#include "GameInstance.h"

CSpikeHole::CSpikeHole(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CHole { pGraphic_Device }
{
}

CSpikeHole::CSpikeHole(const CSpikeHole& Prototype)
	: CHole{ Prototype }
{
}

HRESULT CSpikeHole::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpikeHole::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	m_pTransformCom->Scaling(2.f, 2.f, 2.f);

	_float3 vPosition = pDesc->vPos;
	vPosition.y -= 1.f;

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));

	_float3 vColliderPos = pDesc->vPos;
	vColliderPos.y += 1.f;

	m_pColliderCom->ApplyFixedPosition(vColliderPos);

	name = TEXT("Hole");

	m_bDead = false;

	return S_OK;
}

void CSpikeHole::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
		m_pColliderCom->Collision_Off();
	else if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)
	{
		if (!m_bDead)
			m_pColliderCom->Collision_On();
	}
}

void CSpikeHole::Update(_float fTimeDelta)
{
}

void CSpikeHole::Late_Update(_float fTimeDelta)
{
	if (!m_bDead)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CSpikeHole::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}

HRESULT CSpikeHole::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Spikes"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.vColliderScale = _float3(1.8f, 1.8f, 1.8f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpikeHole::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSpikeHole::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CSpikeHole* CSpikeHole::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpikeHole* pInstance = new CSpikeHole(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : SpikeHole"));
	}

	return pInstance;
}

CGameObject* CSpikeHole::Clone(void* pArg)
{
	CSpikeHole* pInstance = new CSpikeHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : SpikeHole"));
	}

	return pInstance;
}

void CSpikeHole::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
