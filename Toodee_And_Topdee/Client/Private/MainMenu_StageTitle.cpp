#include "MainMenu_StageTitle.h"
#include "GameInstance.h"

CMainMenu_StageTitle::CMainMenu_StageTitle(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CMainMenu_StageTitle::CMainMenu_StageTitle(const CMainMenu_StageTitle& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMainMenu_StageTitle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu_StageTitle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	Ready_SubscribeEvent(m_pGameInstance->Get_NextLevelID());

	return S_OK;
}

void CMainMenu_StageTitle::Priority_Update(_float fTimeDelta)
{
}

void CMainMenu_StageTitle::Update(_float fTimeDelta)
{	
	Change_TextMotion(fTimeDelta);
	Change_ArrowMotion(fTimeDelta);
	Change_Page(fTimeDelta);
}

void CMainMenu_StageTitle::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMainMenu_StageTitle::Render()
{
	HRESULT hr;
	SetUp_RenderState();

	hr = Render_Text();
	if (hr == E_FAIL) return hr;

	hr = Render_Arrow();
	if (hr == E_FAIL) return hr;


	Reset_RenderState();
	return S_OK;
}

void CMainMenu_StageTitle::OnStageSelected(const STAGE_SELECT_EVENT& event)
{
	m_iCurrentStage = event.iNum;
	m_bOnceChangeStage = true;
}

void CMainMenu_StageTitle::OnPageButtons(const MAINMENU_SWAP_EVENT& event)
{
	m_bIsStage = false;
	m_bOncePageChange = true;
	m_fEasedAccumurateTime = 0.f;
}

void CMainMenu_StageTitle::OnPageStage(const MAINMENU_SWAP_EVENT& event)
{
	m_bIsStage = true;
	m_bOncePageChange = true;
	m_fEasedAccumurateTime = 0.f;

}

HRESULT CMainMenu_StageTitle::Ready_SubscribeEvent(_uint iPlayLevel)
{
	m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_BUTTON, [this](const MAINMENU_SWAP_EVENT& Event) {
		this->OnPageButtons(Event);
		});

	m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_STAGE, [this](const MAINMENU_SWAP_EVENT& Event) {
		this->OnPageStage(Event);
		});

	m_pGameInstance->Subscribe<STAGE_SELECT_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_STAGE_SELECT, [this](const STAGE_SELECT_EVENT& Event) {
		this->OnStageSelected(Event);
		});

	return S_OK;
}

void CMainMenu_StageTitle::Change_ArrowMotion(_float fTimeDelta)
{
}

void CMainMenu_StageTitle::Change_TextMotion(_float fTimeDelta)
{
}

void CMainMenu_StageTitle::Change_Page(_float fTimeDelta)
{
	if (!m_bOncePageChange) return;

	m_fEasedAccumurateTime += fTimeDelta;
	_uint iFailed = 0;
	if (m_bIsStage)
	{
		if (EaseVector3InOutBack(&m_vCurrentTextPosition, m_vFadeOutTextPosition, m_vBaseTextPosition, m_fEasedAccumurateTime, 2.f))
			iFailed++;
		if (EaseVector3InOutBack(&m_vCurrentNumberPosition, m_vFadeOutNumberPosition, m_vBaseNumberPosition, m_fEasedAccumurateTime, 2.f))
			iFailed++;
		if (EaseVector3InOutBack(&m_vCurrentArrowLeftPosition, m_vFadeOutArrowLeftPosition, m_vBaseArrowLeftPosition, m_fEasedAccumurateTime, 2.f))
			iFailed++;
		if (EaseVector3InOutBack(&m_vCurrentArrowRightPosition, m_vFadeOutArrowRightPosition, m_vBaseArrowRightPosition, m_fEasedAccumurateTime, 2.f))
			m_bOncePageChange = false;

	}
	else if (!m_bIsStage)
	{
		if (EaseVector3InOutBack(&m_vCurrentTextPosition, m_vBaseTextPosition, m_vFadeOutTextPosition, m_fEasedAccumurateTime, 2.f))
			iFailed++;
		if (EaseVector3InOutBack(&m_vCurrentNumberPosition, m_vBaseNumberPosition, m_vFadeOutNumberPosition, m_fEasedAccumurateTime, 2.f))
			iFailed++;
		if (EaseVector3InOutBack(&m_vCurrentArrowLeftPosition, m_vBaseArrowLeftPosition, m_vFadeOutArrowLeftPosition, m_fEasedAccumurateTime, 2.f))
			iFailed++;
		if (EaseVector3InOutBack(&m_vCurrentArrowRightPosition, m_vBaseArrowRightPosition, m_vFadeOutArrowRightPosition, m_fEasedAccumurateTime, 2.f))
			m_bOncePageChange = false;
	}

	//if (iFailed == 4)m_bOncePageChange = false;
}

HRESULT CMainMenu_StageTitle::Render_Arrow()
{
	m_pTransformCom->Scaling(2.f, 2.f, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, m_vCurrentArrowLeftPosition);
	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom_Arrow->Bind_Texture(1)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	m_pTransformCom->Set_State(STATE::POSITION, m_vCurrentArrowRightPosition);
	m_pTransformCom->Bind_Matrix();
	if (FAILED(m_pTextureCom_Arrow->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CMainMenu_StageTitle::Render_Text()
{
	m_pTransformCom->Scaling(8.f, 8.f, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, m_vCurrentTextPosition);
	m_pTransformCom->Bind_Matrix();

	if (m_iCurrentStage == 3) {
		if (FAILED(m_pTextureCom_Text->Bind_Texture(1)))
			return E_FAIL;
		m_pVIBufferCom->Bind_Buffers();
		//SetUp_RenderState();
		m_pVIBufferCom->Render();
		//Reset_RenderState();
	}
	else if (m_iCurrentStage == 7) {
		if (FAILED(m_pTextureCom_Text->Bind_Texture(2)))
			return E_FAIL;
		m_pVIBufferCom->Bind_Buffers();
		//SetUp_RenderState();
		m_pVIBufferCom->Render();
		//Reset_RenderState();
	}
	else {
		if (FAILED(m_pTextureCom_Text->Bind_Texture(0)))
			return E_FAIL;
		m_pVIBufferCom->Bind_Buffers();
		//SetUp_RenderState();
		m_pVIBufferCom->Render();
		//Reset_RenderState();
		
		m_pTransformCom->Scaling(3.f, 3.f, 1.f);
		m_pTransformCom->Set_State(STATE::POSITION, m_vCurrentNumberPosition);
		m_pTransformCom->Bind_Matrix();
		if (FAILED(m_pTextureCom_Number->Bind_Texture(m_iCurrentStage < 3 ? m_iCurrentStage + 1 : m_iCurrentStage)))
			return E_FAIL;
		m_pVIBufferCom->Bind_Buffers();
		//SetUp_RenderState();
		m_pVIBufferCom->Render();
		//Reset_RenderState();
	}

	return S_OK;
}

HRESULT CMainMenu_StageTitle::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_LevelText"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Text))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Number"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Number))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Arrow"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Arrow))))
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

void CMainMenu_StageTitle::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CMainMenu_StageTitle::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

CMainMenu_StageTitle* CMainMenu_StageTitle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu_StageTitle* pInstance = new CMainMenu_StageTitle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainMenu_StageTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CMainMenu_StageTitle::Clone(void* pArg)
{
	CMainMenu_StageTitle* pInstance = new CMainMenu_StageTitle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMainMenu_StageTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu_StageTitle::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom_Arrow);
	Safe_Release(m_pTextureCom_Text);
	Safe_Release(m_pTextureCom_Number);
}
