#include "PlayerChangeEffect.h"
#include "GameInstance.h"

CPlayerChangeEffect::CPlayerChangeEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPoolableObject{ pGraphic_Device }
{
}

CPlayerChangeEffect::CPlayerChangeEffect(const CPlayerChangeEffect& Prototype)
	:CPoolableObject{ Prototype }
{
}

HRESULT CPlayerChangeEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerChangeEffect::Initialize(void* pArg)
{	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(3.f, 3.f, 1.f);

	name = TEXT("Effect_Player_Change");

	Ready_SubscribeEvent();
	return S_OK;
}

void CPlayerChangeEffect::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CPlayerChangeEffect::Update(_float fTimeDelta)
{
	if (!m_bActive) return;

	Change_Motion(fTimeDelta);
	Change_Color(fTimeDelta);
	Go_Goal(fTimeDelta);
}

void CPlayerChangeEffect::Late_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CPlayerChangeEffect::Render()
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

HRESULT CPlayerChangeEffect::Initialize_Pool(void* pArg)
{
	m_fTravelAccumurateTime = 0.f;
	m_fAccumulateMotionTime = 0.f;
	m_fHue = 0.f;

	return S_OK;
}

void CPlayerChangeEffect::Get_PlayersPosition_Event(const PLAYERSPOSITION_EVENT& event)
{
	if (event.eWho == DIMENSION::TOODEE)
		m_pTransform_Toodee = event.pTransformToodee;
	else
		m_pTransform_Topdee = event.pTransformTopdee;
}

HRESULT CPlayerChangeEffect::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<PLAYERSPOSITION_EVENT>(iPlayerLevel, EVENT_KEY::PLAYERS_POSITION, [this](const PLAYERSPOSITION_EVENT& Event) {
		this->Get_PlayersPosition_Event(Event); });

	return S_OK;
}

HRESULT CPlayerChangeEffect::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture  10 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MainMenu_Spark"),
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

void CPlayerChangeEffect::SetUp_RenderState()
{
	// 알파 블렌딩 설정
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 연하게 만들기 위한 텍스처팩터 설정 (알파값 조정)
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR,m_Color);

	// 텍스처 색상과 팩터를 곱해서 연하게 만들기
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	// 알파 값도 텍스처와 팩터를 곱해서 부드럽게
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

}

void CPlayerChangeEffect::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

void CPlayerChangeEffect::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
	{
		m_fAccumulateMotionTime = 0.f;
		m_iMotionNum = ++m_iMotionNum == 10 ? 0 : m_iMotionNum;
	}
	else
		m_fAccumulateMotionTime += fTimeDelta;
}

void CPlayerChangeEffect::Change_Color(_float fTimeDelta)
{
	m_fHue += m_fColorSpeed * fTimeDelta;  
	if (m_fHue >= 360.0f) m_fHue -= 360.0f;

	m_Color = HSVtoARGB(m_fHue, 1.0f, 1.0f);
}

void CPlayerChangeEffect::Go_Goal(_float fTimeDelta)
{
	//if (m_pTransform_Toodee == nullptr) return;
	m_fTravelAccumurateTime += fTimeDelta;
	if (m_pGameInstance->Get_PreviousDimension() == DIMENSION::TOPDEE) {
		if (EaseVector3InOutBack(&m_vCurPosition, m_pTransform_Topdee->Get_State(STATE::POSITION), m_pTransform_Toodee->Get_State(STATE::POSITION), m_fTravelAccumurateTime, m_fTravelTime, 2.3f))
		{
			m_pGameInstance->Push(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_PlayerChangeEffect"), this);
		}
	}
	else
		if (EaseVector3InOutBack(&m_vCurPosition, m_pTransform_Toodee->Get_State(STATE::POSITION), m_pTransform_Topdee->Get_State(STATE::POSITION), m_fTravelAccumurateTime, m_fTravelTime, 2.3f))
		{
			m_pGameInstance->Push(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_PlayerChangeEffect"), this);
			}

	m_pTransformCom->Set_State(STATE::POSITION, m_vCurPosition);

}

D3DCOLOR CPlayerChangeEffect::HSVtoARGB(_float h, _float s, _float v, _float a)
{
	_float r, g, b;

	_int i = _int(h / 60.0f) % 6;
	_float f = (h / 60.0f) - i;
	_float p = v * (1.0f - s);
	_float q = v * (1.0f - f * s);
	_float t = v * (1.0f - (1.0f - f) * s);

	switch (i)
	{
	case 0: r = v; g = t; b = p; break;
	case 1: r = q; g = v; b = p; break;
	case 2: r = p; g = v; b = t; break;
	case 3: r = p; g = q; b = v; break;
	case 4: r = t; g = p; b = v; break;
	case 5: r = v; g = p; b = q; break;
	}

	return D3DCOLOR_ARGB(
		int(a * 255),
		int(r * 255),
		int(g * 255),
		int(b * 255)
	);
}

CPlayerChangeEffect* CPlayerChangeEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerChangeEffect* pInstance = new CPlayerChangeEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayerChangeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CPlayerChangeEffect::Clone(void* pArg)
{
	CPlayerChangeEffect* pInstance = new CPlayerChangeEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlayerChangeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerChangeEffect::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
