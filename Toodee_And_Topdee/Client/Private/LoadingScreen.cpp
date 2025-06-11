#include "LoadingScreen.h"
#include "GameInstance.h"

CLoadingScreen::CLoadingScreen(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CLoadingScreen::CLoadingScreen(const CLoadingScreen& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CLoadingScreen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingScreen::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	Ready_SubscribeEvent(ENUM_CLASS(LEVEL::LEVEL_STATIC));


	//D3DXFONT_DESC fontDesc = {};
	//fontDesc.Height = 24;
	//fontDesc.Width = 10;
	//fontDesc.Weight = FW_NORMAL;
	//fontDesc.Italic = false;
	//fontDesc.CharSet = DEFAULT_CHARSET;
	//fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	//fontDesc.Quality = DEFAULT_QUALITY;
	//fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	//lstrcpy(fontDesc.FaceName, TEXT("Arial")); 

	//D3DXCreateFontIndirect(m_pGraphic_Device, &fontDesc, &m_pFont);

	return S_OK;
}

void CLoadingScreen::Priority_Update(_float fTimeDelta)
{
}

void CLoadingScreen::Update(_float fTimeDelta)
{

	if (m_bStartScreen ^ m_bEndScreen)
		Motion_Change(fTimeDelta);


}

void CLoadingScreen::Late_Update(_float fTimeDelta)
{

	//if (m_bStartScreen ^ m_bEndScreen)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CLoadingScreen::Render()
{
	if(m_bStartScreen ^ m_bEndScreen)
	{
		m_pTransformCom->Bind_Matrix();

		if (FAILED(m_pTextureCom->Bind_Texture(m_iframeKey)))
			return E_FAIL;

		m_pVIBufferCom->Bind_Buffers();

		SetUp_RenderState();

		m_pVIBufferCom->Render();

		Reset_RenderState();
	}

	//RECT rc;
	//SetRect(&rc, 10, 10, 300, 100);  // 출력 위치

	//TCHAR szText[64];
	//_stprintf_s(szText, TEXT("Pos: X= %.2f, Y= %.2f, Z= %.2f"), m_vPosition.x, m_vPosition.y, m_vPosition.z);
	//m_pFont->DrawText(NULL, szText, -1, &rc, DT_LEFT | DT_TOP, D3DCOLOR_ARGB(255, 255, 0, 0));


	return S_OK;
}

void CLoadingScreen::OnStartScreen(const LOADINGSCREEN_EVENT& event)
{

	if (m_pGameInstance->Get_NextLevelID() >= ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1)) return;

	m_bFadeIn = event.bFadeIn;

	m_vPosition = event.vPos;

	if (m_bFadeIn)
	{
		m_bStartScreen = true;
		m_bEndScreen = false;
		m_iframeKey = 0;
	}
	else
	{
		m_bStartScreen = false;
		m_bEndScreen = true;
		m_iframeKey = 12;
	}

	//D3DXMATRIX viewMatrix, cameraMatrix;
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMatrix);
	//D3DXMatrixInverse(&cameraMatrix, nullptr, &viewMatrix);
	//m_pTransformCom->Set_Matrix(cameraMatrix);

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);
	m_pTransformCom->Bind_Matrix();

	//_float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
	//m_vPosition = pos;

	//if (m_pGameInstance->Get_NextLevelID() <  ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1))
	//{
	//	m_pTransformCom->Scaling(1.f,1.f,1.f);
	//	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	//	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	//}
	//else
	//{
	//	//m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);
	//	m_pTransformCom->Scaling(1.f, 1.f, 1.f);

	//	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	//	m_vPosition.y -= 10.f;
	//	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
	//}

	m_fAccumurateTime = 0.f;
	m_FadeAccumurateTime = 0.f;
}

void CLoadingScreen::Motion_Change(_float fTimeDelta)
{
	if (m_FadeAccumurateTime+ fTimeDelta > 0.6f) {

		if (m_fIntervalTime <= m_fAccumurateTime + fTimeDelta)
		{
			m_fAccumurateTime = 0.f;

			if (m_bFadeIn)
			{
				++m_iframeKey;
				if (m_iframeKey > 12) m_bStartScreen = false;
			}
			else
			{
				--m_iframeKey;
				if (m_iframeKey < 0) m_bEndScreen = false;
			}
		}
		else
			m_fAccumurateTime += fTimeDelta;

	}
	else
		m_FadeAccumurateTime += fTimeDelta;
}

HRESULT CLoadingScreen::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_LoadingScreen"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	return S_OK;
}

void CLoadingScreen::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

}

void CLoadingScreen::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CLoadingScreen::Ready_SubscribeEvent(_uint iPlayLevel)
{
	m_pGameInstance->Subscribe<LOADINGSCREEN_EVENT>(iPlayLevel, EVENT_KEY::LOADINGSCREEN, [this](const LOADINGSCREEN_EVENT& Event) {
		this->OnStartScreen(Event);
		});

	return S_OK;
}

CLoadingScreen* CLoadingScreen::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoadingScreen* pInstance = new CLoadingScreen(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLoadingScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CLoadingScreen::Clone(void* pArg)
{
	CLoadingScreen* pInstance = new CLoadingScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLoadingScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingScreen::Free()
{
	__super::Free();

	//Safe_Release(m_pFont);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
