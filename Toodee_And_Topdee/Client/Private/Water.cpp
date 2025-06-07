#include "Water.h"
#include "GameInstance.h"

CWater::CWater(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CWater::CWater(const CWater& Prototype)
	: CGameObject { Prototype }
{
}

HRESULT CWater::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;;

	WATER_DESC* pDesc = static_cast<WATER_DESC*>(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_float3 vPosition = m_vSinkPos = pDesc->vPosition;

	vPosition.x -= 10.f;
	vPosition.y = 1.1f;
	vPosition.z -= 18.f;

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);

	if(nullptr != pDesc->ppLink)
	{
		*(pDesc->ppLink) = this;
		Safe_AddRef(*pDesc->ppLink);
	}

	m_fCurrentFillHeight = 0.f;
	m_fFillHeight = 0.f;
	m_fCurrentStrength = 0.f;
	m_fSpeed = 15.f;
	m_fTimeDelta = 0.f;

	m_pColliderCom->Collision_Off();

	return S_OK;
}

void CWater::Priority_Update(_float fTimeDelta)
{
	Check_Dimension();
}

void CWater::Update(_float fTimeDelta)
{
	if (m_eCurrentDimension == DIMENSION::TOODEE)
	{
		m_fTimeDelta += fTimeDelta;

		if (m_fCurrentFillHeight >= m_fFillHeight)
		{
			m_fCurrentFillHeight = m_fFillHeight;

		}
		else
			m_fCurrentFillHeight += (m_fFillHeight * fTimeDelta);
		
		Collider_Setting();
	}
	else if (m_eCurrentDimension == DIMENSION::TOPDEE)
	{
		if (m_fCurrentStrength >= 1.f)
		{
			m_fCurrentStrength = 1.f;
		}
		else
		{
			m_fCurrentStrength += fTimeDelta;
			m_fTimeDelta += fTimeDelta;
		}
	}
	else
	{

	}
}

void CWater::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CWater::Render()
{
	m_pColliderCom->Render();

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();
	Begin_ShaderState();

	m_pVIBufferCom->Render();

	End_RenderState();
	End_ShaderState();

	return S_OK;
}

void CWater::FillWater(_float fFillHeight)
{
	m_bFill = true;
	m_fFillHeight = fFillHeight;
}

void CWater::Check_Dimension()
{
	DIMENSION eNewDimension = m_pGameInstance->Get_CurrentDimension();

	if (m_eCurrentDimension == eNewDimension)
		return;

	if (m_eCurrentDimension == DIMENSION::TOPDEE)
	{
		m_fCurrentFillHeight = 0.f;
	}

	m_eCurrentDimension = eNewDimension;
	
	if (m_eCurrentDimension == DIMENSION::TOPDEE)
	{
		m_pColliderCom->Collision_Off();

		m_fCurrentStrength = 0.f;
	}
	else if (m_eCurrentDimension == DIMENSION::TOODEE)
	{
		m_pColliderCom->Collision_On();

		m_fCurrentStrength = 0.f;
	}

	m_fTimeDelta = 0.f;
}

void CWater::Collider_Setting()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	vPosition.x += 10.f;
	vPosition.z += (m_fCurrentFillHeight * 18.f) * 0.5f; // Water Size Z
	m_pColliderCom->ApplyFixedPosition(vPosition);
	m_pColliderCom->Set_Scaling(20.f, 2.f, (m_fCurrentFillHeight * 18.f));
}

HRESULT CWater::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_Texture_Water"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.vColliderScale = _float3(18.f, 2.f, 18.f);

	if(FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_WaterCollider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_Shader_Water"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CWater::Begin_ShaderState()
{
	CShader_Water::SHADERWATER_DESC pDesc{};
	_float4x4 WorldMatrix = *m_pTransformCom->Get_WorldMatrix();
	_float4x4 ViewMatrix;
	_float4x4 ProjMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	pDesc.WorldMatrix = WorldMatrix;
	pDesc.ViewMatrix = ViewMatrix;
	pDesc.ProjMatrix = ProjMatrix;

	pDesc.fFillHeight = m_fCurrentFillHeight;
	pDesc.vSinkPosition = m_vSinkPos;
	pDesc.fSinkPower = m_fCurrentStrength;
	pDesc.fTimeDelta = m_fTimeDelta;
	pDesc.fSpeed = m_fSpeed;

	m_pShader->Bind_Shader(&pDesc);

	return S_OK;
}

HRESULT CWater::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CWater::End_ShaderState()
{
	m_pShader->Reset_Shader();

	return S_OK;
}

CWater* CWater::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWater* pInstance = new CWater(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CWater"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CWater::Clone(void* pArg)
{
	CWater* pInstance = new CWater(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CWater"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShader);
}
