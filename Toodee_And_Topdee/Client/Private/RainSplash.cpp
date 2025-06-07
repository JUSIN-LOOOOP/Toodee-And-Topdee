#include "RainSplash.h"
#include "GameInstance.h"

CRainSplash::CRainSplash(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPoolableObject{ pGraphic_Device }
{
}

CRainSplash::CRainSplash(const CRainSplash& Prototype)
	:CPoolableObject{ Prototype }
{
}

HRESULT CRainSplash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRainSplash::Initialize(void* pArg)
{	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(1.4f, 2.f, 1.f);

	m_fMotionIntervalTime = { 0.2f };

	name = TEXT("RainSplash");

	return S_OK;
}

void CRainSplash::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CRainSplash::Update(_float fTimeDelta)
{
	if (!m_bActive) return;

	if (m_pGameInstance->Get_CurrentDimension() != DIMENSION::TOODEE)
		m_pGameInstance->Push(TEXT("Layer_RainSplash"), this);

	Change_Motion(fTimeDelta);
}

void CRainSplash::Late_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CRainSplash::Render()
{
	if (!m_bActive)
		return S_OK;

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iMotionNum)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	return S_OK;
}

HRESULT CRainSplash::Initialize_Pool(void* pArg)
{
	_float3* info = reinterpret_cast<_float3*>(pArg);
	m_vStartPosition = *info;
	m_pTransformCom->Set_State(STATE::POSITION, m_vStartPosition);
	
	m_iMotionNum = static_cast<_uint>(m_pGameInstance->Rand(1.f, 7.f));
	m_fAccumulateMotionTime = { 0.f };

	//m_pTransformCom->Go_Backward(0.06f);

	return S_OK;
}

HRESULT CRainSplash::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_RainSplash"),
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

void CRainSplash::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CRainSplash::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CRainSplash::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
	{
		m_fAccumulateMotionTime = 0.f;
		m_iMotionNum = ++m_iMotionNum == 6 ? 0 : m_iMotionNum;
	}
	else
		m_fAccumulateMotionTime += fTimeDelta;
}

CRainSplash* CRainSplash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRainSplash* pInstance = new CRainSplash(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRainSplash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CRainSplash::Clone(void* pArg)
{
	CRainSplash* pInstance = new CRainSplash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRainSplash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRainSplash::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
