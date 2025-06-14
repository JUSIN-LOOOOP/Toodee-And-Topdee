#include "MainMenu_Buttons.h"
#include "GameInstance.h"

CMainMenu_Buttons::CMainMenu_Buttons(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CMainMenu_Buttons::CMainMenu_Buttons(const CMainMenu_Buttons& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMainMenu_Buttons::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu_Buttons::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_vButton_MainMenu_Position[0] = m_vButton_Position[0] = { 0.f, 1.f, -8.f };
	m_vButton_MainMenu_Position[1] = m_vButton_Position[1] = { 0.f, 1.f, -12.f };
	m_vButton_Stage_Position[0] = { 0.f, 1.f, 40.f };
	m_vButton_Stage_Position[1] = { 0.f, 1.f, 40.f };

	m_vButton_Size = { 10.f,4.f,1.f };

	m_fArrow_OffsetX = { 10.f };
	m_vArrow_Size = { 2.f, 2.f,1.f };

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

	//if (FAILED(Ready_SubscribeEvent(m_iPlayLevel)))
	//	return E_FAIL;

	return S_OK;
}

void CMainMenu_Buttons::Priority_Update(_float fTimeDelta)
{
}

void CMainMenu_Buttons::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(VK_DOWN)) {
		m_iCurrentButton = --m_iCurrentButton < 0 ? 1 : 0;
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("Menu_Pick1.wav"), CHANNELID::SOUND_MENU, 0.5f);
	}
	if (m_pGameInstance->Key_Down(VK_UP)) {
		m_iCurrentButton = ++m_iCurrentButton > 1 ? 0 : 1;
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("Menu_Pick2.wav"), CHANNELID::SOUND_MENU, 0.5f);

	}

	Change_Motion(fTimeDelta);

	// MainMenu -> Select Stage
	if (m_bIsMainMenu && m_pGameInstance->Key_Down(VK_RETURN) && m_iCurrentButton == 0) {
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("Menu_Picked1.wav"), CHANNELID::SOUND_MENU, 0.5f);


		MAINMENU_SWAP_EVENT Event;
		m_bIsMainMenu = false;
		m_bArrivalPosition = false;
		m_easedTime = 0.f;
		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::MAINMENU_STAGE, Event);
	}

	//Exit
	if (m_pGameInstance->Key_Down(VK_RETURN) && m_iCurrentButton == 1) {
		
		//todo 나가기 
	}

	//Select Stage -> MainMenu 
	if (!m_bIsMainMenu && m_pGameInstance->Key_Down(VK_ESCAPE))
	{
		m_pGameInstance->StopSound(CHANNELID::SOUND_MENU);
		m_pGameInstance->PlayAudio(TEXT("Menu_Picked2.wav"), CHANNELID::SOUND_MENU, 0.5f);

		MAINMENU_SWAP_EVENT Event;
		m_bIsMainMenu = true;
		m_bArrivalPosition = false;
		m_easedTime = 0.f;
		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::MAINMENU_BUTTON, Event);
	}

	// MainMenu -> Select Stage
	if (!m_bIsMainMenu && !m_bArrivalPosition)
	{
		m_easedTime += fTimeDelta;
		for (_uint i = 0; i < 2; ++i)
			if (EaseVector3InOutBack(&m_vButton_Position[i], m_vButton_MainMenu_Position[i], m_vButton_Stage_Position[i], m_easedTime, 2.f))
				m_bArrivalPosition = true;
	}

	//Select Stage -> MainMenu 
	if (m_bIsMainMenu && !m_bArrivalPosition)
	{
		m_easedTime += fTimeDelta;
		for (_uint i = 0; i < 2; ++i)
			if (EaseVector3InOutBack(&m_vButton_Position[i], m_vButton_Stage_Position[i], m_vButton_MainMenu_Position[i], m_easedTime, 2.f))
				m_bArrivalPosition = true;
	}

}

void CMainMenu_Buttons::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMainMenu_Buttons::Render()
{

	for (_uint i = 0; i < 2; ++i) {
		m_pTransformCom->Set_State(STATE::POSITION, m_vButton_Position[i]);
		m_pTransformCom->Scaling(m_vButton_Size.x, m_vButton_Size.y, m_vButton_Size.z);
		m_pTransformCom->Bind_Matrix();
		if (FAILED(m_pTextureCom_Buttons->Bind_Texture(i)))
			return E_FAIL;

		if (i == m_iCurrentButton)
		{
			m_pVIBufferCom->Bind_Buffers();
			SetUp_GreenRenderState();
			m_pVIBufferCom->Render();

			m_pTransformCom->Set_State(STATE::POSITION, _float3(m_vButton_Position[i].x - m_fMotionArrowPositionX - m_fArrow_OffsetX , m_vButton_Position[i].y, m_vButton_Position[i].z));
			m_pTransformCom->Scaling(m_vArrow_Size.x, m_vArrow_Size.y, m_vArrow_Size.z);
			m_pTransformCom->Bind_Matrix();
			if (FAILED(m_pTextureCom_Arrow->Bind_Texture(0)))
				return E_FAIL;
			m_pVIBufferCom->Bind_Buffers();
			SetUp_GreenRenderState();
			m_pVIBufferCom->Render();


			m_pTransformCom->Set_State(STATE::POSITION, _float3(m_vButton_Position[i].x + m_fMotionArrowPositionX + m_fArrow_OffsetX, m_vButton_Position[i].y, m_vButton_Position[i].z));
			m_pTransformCom->Bind_Matrix();
			if (FAILED(m_pTextureCom_Arrow->Bind_Texture(1)))
				return E_FAIL;
			m_pVIBufferCom->Bind_Buffers();
			SetUp_GreenRenderState();
			m_pVIBufferCom->Render();

			Reset_GreenRenderState();

		}
		else
		{
			m_pVIBufferCom->Bind_Buffers();
			SetUp_RenderState();
			m_pVIBufferCom->Render();
			Reset_RenderState();
		}


	}


	return S_OK;
}

void CMainMenu_Buttons::OnPageUp(const MAINMENU_SWAP_EVENT& event)
{
	
}

void CMainMenu_Buttons::OnPageDown(const MAINMENU_SWAP_EVENT& event)
{
}

void CMainMenu_Buttons::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fMotionAccumurateTime + fTimeDelta)
	{
		m_fMotionAccumurateTime = 0.f;
		if (m_bMotionArrow && m_fMotionArrowPositionX >= 2.f)
			m_bMotionArrow = false;
		else if (!m_bMotionArrow && m_fMotionArrowPositionX <= - 2.f)
			m_bMotionArrow = true;

		if (m_bMotionArrow)
			m_fMotionArrowPositionX += fTimeDelta * 15.f;
		else
			m_fMotionArrowPositionX -= fTimeDelta * 15.f;

	}
	else
		m_fMotionAccumurateTime += fTimeDelta;
}

HRESULT CMainMenu_Buttons::Ready_SubscribeEvent(_uint iPlayLevel)
{
	m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(m_iPlayLevel, EVENT_KEY::MAINMENU_BUTTON, [this](const MAINMENU_SWAP_EVENT& Event) {
		this->OnPageUp(Event);
		});

	m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(m_iPlayLevel, EVENT_KEY::MAINMENU_STAGE, [this](const MAINMENU_SWAP_EVENT& Event) {
		this->OnPageDown(Event);
		});

	return S_OK;
}

HRESULT CMainMenu_Buttons::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Buttons"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Buttons))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Arrow"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Arrow))))
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

void CMainMenu_Buttons::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
}

void CMainMenu_Buttons::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
}

void CMainMenu_Buttons::SetUp_GreenRenderState()
{
	// 알파 블렌딩 활성화
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//초록색
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 0, 255, 0));

	// 텍스처와 색상 곱하기
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	// 알파도 텍스처와 TFACTOR 곱하기
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}

void CMainMenu_Buttons::Reset_GreenRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

CMainMenu_Buttons* CMainMenu_Buttons::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu_Buttons* pInstance = new CMainMenu_Buttons(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainMenu_Buttons"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CMainMenu_Buttons::Clone(void* pArg)
{
	CMainMenu_Buttons* pInstance = new CMainMenu_Buttons(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMainMenu_Buttons"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu_Buttons::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Buttons);
	Safe_Release(m_pTextureCom_Arrow);

}
