#include "MainMenu_Portals.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CMainMenu_Portals::CMainMenu_Portals(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CMainMenu_Portals::CMainMenu_Portals(const CMainMenu_Portals& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMainMenu_Portals::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu_Portals::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	for (_uint i = 0; i < 8; ++i)
	{
		m_vPosition[i] = m_vPageButtonPosition[i] = { static_cast<_float>(i) * m_fIntervalX , 1.1f, -40.f};
		m_vPageStagePosition[i] = { static_cast<_float>(i) * m_fIntervalX, 1.f, 0.f };
	}

	m_vColor[0][0] = D3DCOLOR_ARGB(255, 255, 182, 193);
	m_vColor[0][1] = D3DCOLOR_ARGB(255, 255, 182, 193);
	m_vColor[0][2] = D3DCOLOR_ARGB(255, 124, 252, 0);
	m_vColor[0][3] = D3DCOLOR_ARGB(255, 255, 69, 0);
	m_vColor[0][4] = D3DCOLOR_ARGB(255, 124, 252, 0);
	m_vColor[0][5] = D3DCOLOR_ARGB(255, 128, 128, 128);
	m_vColor[0][6] = D3DCOLOR_ARGB(255, 128, 128, 128);
	m_vColor[0][7] = D3DCOLOR_ARGB(255, 255, 69, 0);

	for (_uint i = 0; i < 8; ++i)
	{

		BYTE r = (m_vColor[0][i] >> 16) & 0xFF;
		BYTE g = (m_vColor[0][i] >> 8) & 0xFF;
		BYTE b = (m_vColor[0][i]) & 0xFF;

		r = r == 255 ? 255 : r - 10;
		g = g == 255 ? 255 : g - 10;
		b = b==0? 0: b - 10 ;
		m_vColor[1][i] = D3DCOLOR_ARGB(205, r, g, b);

		r = r == 255 ? 255 : r - 20;
		g = g == 255 ? 255 : g - 20;
		b = b == 0 ? 0 : b - 20;
		m_vColor[2][i] = D3DCOLOR_ARGB(160, r, g, b);

	}

	Ready_SubscribeEvent(m_pGameInstance->Get_NextLevelID());

	return S_OK;
}

void CMainMenu_Portals::Priority_Update(_float fTimeDelta)
{
}

void CMainMenu_Portals::Update(_float fTimeDelta)
{
	//키 입력 ( 스테이지 선택 )
	if (m_bIsStage &&m_pGameInstance->Key_Down(VK_RIGHT) && m_iCurSelected < 7 && !m_bChangeStage)
	{
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("Menu_Picked1.wav"), CHANNELID::SOUND_MENU, 0.5f);

		m_fEasedTime = 0.f;
		++m_iCurSelected;
		m_bChangeStage = true;

		STAGE_SELECT_EVENT event;
		event.iNum = m_iCurSelected;
		m_pGameInstance->Publish(m_pGameInstance->Get_NextLevelID(), EVENT_KEY::MAINMENU_STAGE_SELECT, event);
	}
	if (m_bIsStage && m_pGameInstance->Key_Down(VK_LEFT) && m_iCurSelected > 0 && !m_bChangeStage)
	{
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("Menu_Picked2.wav"), CHANNELID::SOUND_MENU, 0.5f);

		m_fEasedTime = 0.f;
		--m_iCurSelected;
		m_bChangeStage = true;

		STAGE_SELECT_EVENT event;
		event.iNum = m_iCurSelected;
		m_pGameInstance->Publish(m_pGameInstance->Get_NextLevelID(), EVENT_KEY::MAINMENU_STAGE_SELECT, event);
	}

	//스테이지 입장
	if (m_bIsStage && m_pGameInstance->Key_Down('Z'))
	{
	/*	if (m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, static_cast<LEVEL>(m_iCurSelected + 4))))
			return;*/
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("MenuStageJoin.wav"), CHANNELID::SOUND_MENU, 0.6f);

		LEVELCHANGE_EVENT Event;
		Event.iChangeLevel = m_iCurSelected + 4;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_LOGO);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);

	}

	Apply_Selected_Position(fTimeDelta);
	Change_Page(fTimeDelta);
	Change_Motion(fTimeDelta);
}

void CMainMenu_Portals::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMainMenu_Portals::Render()
{
	HRESULT hr{};

	hr = Render_StageImage();
	if (hr == E_FAIL)
		return hr;

	hr = Render_Portal();
	if (hr == E_FAIL)
		return hr;

	return S_OK;
}

void CMainMenu_Portals::OnPageButtons(const MAINMENU_SWAP_EVENT& event)
{
	m_bIsStage = false;
	m_bPageOnceChange = true;
	m_fEasedPageTime = 0.f;
}

void CMainMenu_Portals::OnPageStage(const MAINMENU_SWAP_EVENT& event)
{
	m_bIsStage = true;
	m_bPageOnceChange = true;
	m_fEasedPageTime = 0.f;
}

HRESULT CMainMenu_Portals::Ready_SubscribeEvent(_uint iPlayLevel)
{
	m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_BUTTON, [this](const MAINMENU_SWAP_EVENT& Event) {
		this->OnPageButtons(Event);
		});

	m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_STAGE, [this](const MAINMENU_SWAP_EVENT& Event) {
		this->OnPageStage(Event);
		});

	return S_OK;
}

HRESULT CMainMenu_Portals::Render_Portal()
{	
	m_pTransformCom->Scaling(10.f, 10.f, 1.f);
	for (_int i = 0; i < 8; ++i)
	{
		//if () continue;
		m_vPosition[i].y += 0.1;
		m_pTransformCom->Set_State(STATE::POSITION, m_vPosition[i]);
		m_pTransformCom->Bind_Matrix();
		
		for (_uint j = 0; j < 3; ++j)
		{
			if (FAILED(m_pTextureCom_Portal->Bind_Texture(m_iMotionNum[j])))
				return E_FAIL;
			m_pVIBufferCom->Bind_Buffers();
			SetUp_ColorRenderState(j,i);
			m_pVIBufferCom->Render();
			Reset_ColorRenderState();
		}

	}		
	return S_OK;
}

HRESULT CMainMenu_Portals::Render_StageImage()
{
	SetUp_RenderState();
	m_pTransformCom->Scaling(8.2f, 8.2f, 1.f);
	for (_int i = 0; i < 8; ++i)
	{
		m_vPosition[i].y -= 0.1;
		m_pTransformCom->Set_State(STATE::POSITION, m_vPosition[i]);
		m_pTransformCom->Bind_Matrix();
		if (FAILED(m_pTextureCom_StageImage->Bind_Texture(i)))
			return E_FAIL;
		m_pVIBufferCom->Bind_Buffers();

		m_pVIBufferCom->Render();

	}
	Reset_RenderState();
	return S_OK;
}

void CMainMenu_Portals::Change_Page(_float fTimeDelta)
{
	if (!m_bPageOnceChange) return;

	m_fEasedPageTime += fTimeDelta;

	if (!m_bIsStage)
	{
		for (_uint i = 0; i < 8; ++i)
			if (EaseFloatInOutBack(&m_vPosition[i].z, m_vPageStagePosition[i].z, m_vPageButtonPosition[i].z, m_fEasedPageTime, 1.8f))
				m_bPageOnceChange = false;
	}

	if (m_bIsStage)
	{
		for(_uint i = 0 ; i <8 ;++i)
			if (EaseFloatInOutBack(&m_vPosition[i].z, m_vPageButtonPosition[i].z, m_vPageStagePosition[i].z, m_fEasedPageTime, 1.8f))
				m_bPageOnceChange = false;
	}
	

}

void CMainMenu_Portals::Apply_Selected_Position(_float fTimeDelta)
{
	if (m_bChangeStage) {
		m_fEasedTime += fTimeDelta;
		for (_uint i = 0; i < 8; ++i)
		{
			if (EaseFloatOutBounce(&m_vPosition[i].x, m_vPosition[i].x, m_vPageStagePosition[i].x - static_cast<_float>(m_iCurSelected) * m_fIntervalX, m_fEasedTime, 0.3f))
				m_bChangeStage = false;

		}
	}
}

void CMainMenu_Portals::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
	{
		m_fAccumulateMotionTime = 0.f;
		for(_uint i = 0 ; i <3 ;++i)
			m_iMotionNum[i] = ++m_iMotionNum[i] >= 10 ? 0 : m_iMotionNum[i];
	}
	else
		m_fAccumulateMotionTime += fTimeDelta;
}

HRESULT CMainMenu_Portals::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_StagePortal"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Portal))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_StageImage"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_StageImage))))
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

void CMainMenu_Portals::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CMainMenu_Portals::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CMainMenu_Portals::SetUp_ColorRenderState(_uint iNum, _uint iStage)
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, m_vColor[iNum][iStage]);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}

void CMainMenu_Portals::Reset_ColorRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

CMainMenu_Portals* CMainMenu_Portals::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu_Portals* pInstance = new CMainMenu_Portals(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainMenu_Portals"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CMainMenu_Portals::Clone(void* pArg)
{
	CMainMenu_Portals* pInstance = new CMainMenu_Portals(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMainMenu_Portals"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu_Portals::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Portal);
	Safe_Release(m_pTextureCom_StageImage);

}
