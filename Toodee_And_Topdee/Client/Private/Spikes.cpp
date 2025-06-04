#include "Spikes.h"
#include "GameInstance.h"

CSpikes::CSpikes(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject {pGraphic_Device }
{
}

CSpikes::CSpikes(const CSpikes& Prototype)
	: CGameObject { Prototype }
	, m_fAnimDelay { Prototype.m_fAnimDelay }
	, m_iMaxAnimCount {Prototype.m_iMaxAnimCount }
{
}

HRESULT CSpikes::Initialize_Prototype()
{
	name = TEXT("Enemy_Spikes");

	m_fAnimDelay = 0.08f;
	m_iMaxAnimCount = 5;

	return S_OK;
}

HRESULT CSpikes::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_eRenderDimension = m_pGameInstance->Get_CurrentDimension();
	
	switch (m_eRenderDimension)
	{
	case DIMENSION::TOODEE:
		m_iAnimCount = 0;
		break;
	case DIMENSION::TOPDEE:
		m_iAnimCount = 5;
		break;
	}

	m_bChange_Dimension = false;
	
	m_fCurrentAnimTime = 0.f;
	
	m_iAnimCount = 0;

	m_iCount = 0;

	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	m_pTransformCom->Set_State(STATE::POSITION, _float3(7.f, 0.1f, 7.f));
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	return S_OK;
}

void CSpikes::Priority_Update(_float fTimeDelta)
{
	Check_Dimension();
}

void CSpikes::Update(_float fTimeDelta)
{
}

void CSpikes::Late_Update(_float fTimeDelta)
{
	if (m_bChange_Dimension)
		Update_AnimCount(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CSpikes::Render()
{
	m_pColliderCom->Render();

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iAnimCount)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();
	


	return S_OK;
}


void CSpikes::Check_Dimension()
{
	if (m_eRenderDimension != m_pGameInstance->Get_CurrentDimension())
	{
		m_bChange_Dimension = true;
		
		switch (m_eRenderDimension)
		{
		case DIMENSION::TOODEE:
			m_iIncreaseAnimCount = 1;
			break;
		case DIMENSION::TOPDEE:
			m_iIncreaseAnimCount = -1;
			break;
		}

		m_eRenderDimension = m_pGameInstance->Get_CurrentDimension();
	}

}

void CSpikes::Update_AnimCount(_float fTimeDelta)
{
	if (m_iCount >= 5)
	{
		m_bChange_Dimension = false;
		m_iCount = 0;
		return;
	}

	if (m_fCurrentAnimTime + fTimeDelta >= m_fAnimDelay)
	{
		m_iAnimCount += m_iIncreaseAnimCount;
		m_fCurrentAnimTime = 0.f;
		m_iCount++;
	}
	else
		m_fCurrentAnimTime += fTimeDelta;
}

HRESULT CSpikes::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Spikes"),
		TEXT("Com_Textrue"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 2.f, 1.5f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpikes::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSpikes::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CSpikes* CSpikes::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpikes* pInstance = new CSpikes(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CSpikes "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpikes::Clone(void* pArg)
{
	CSpikes* pInstance = new CSpikes(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CSpikes "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpikes::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCom);
}
