#include "Rain.h"
#include "GameInstance.h"

CRain::CRain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPoolableObject{ pGraphic_Device }
{
}

CRain::CRain(const CRain& Prototype)
	:CPoolableObject{ Prototype }
{
}

HRESULT CRain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRain::Initialize(void* pArg)
{	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.2f, 1.f, 0.4f);
	//m_pTransformCom->Scaling(50.f, 10.f, 5.f);

	name = TEXT("Rain");

	m_iLevel = m_pGameInstance->Get_NextLevelID();
	return S_OK;
}

void CRain::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CRain::Update(_float fTimeDelta)
{
	if (!m_bActive) return;

	//m_pTransformCom->Go_Down(fTimeDelta*2.f);

	if (m_pTransformCom->Approach(m_vCrashPosition, fTimeDelta, 25.f))
	{
		_float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
		m_pGameInstance->Push(m_iLevel, TEXT("Layer_Rain"), this);
	}
}

void CRain::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CRain::Render()
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

HRESULT CRain::Initialize_Pool(void* pArg)
{
	RAIN* info = reinterpret_cast<RAIN*>(pArg);
	m_vStartPosition = info->vStartPosition;
	m_vCrashPosition = info->vCrashPosition;

	m_pTransformCom->Set_State(STATE::POSITION, m_vStartPosition);
	if(m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	else
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(45.f));


	return S_OK;
}

HRESULT CRain::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_Component_Texture_Rain"),
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

void CRain::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CRain::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

CRain* CRain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRain* pInstance = new CRain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRain::Clone(void* pArg)
{
	CRain* pInstance = new CRain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRain::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
