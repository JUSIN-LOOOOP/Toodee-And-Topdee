#include "MainMenu_Title.h"
#include "GameInstance.h"

CMainMenu_Title::CMainMenu_Title(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CMainMenu_Title::CMainMenu_Title(const CMainMenu_Title& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMainMenu_Title::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu_Title::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    Ready_SubscribeEvent(m_pGameInstance->Get_NextLevelID());
	return S_OK;
}

void CMainMenu_Title::Priority_Update(_float fTimeDelta)
{
}

void CMainMenu_Title::Update(_float fTimeDelta)
{
    Change_Motion(fTimeDelta);
    Change_Page(fTimeDelta);
}

void CMainMenu_Title::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMainMenu_Title::Render()
{

    //  TOODEE 텍스트 
    _float temp1 = fTextHeight - m_fMotionSize;
    if (temp1 < 10.f)
        temp1 = 10.f;

    m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, fBaseY , fToodeeZ + m_fMotionPosition+ fPagePositionZ));
    m_pTransformCom->Scaling(fTextWidth, temp1, 1.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();

    // 2. TOODEE 그림자

    // Toodee글씨 아래 위치 =  (글씨 맨 아래 위치 - 실제 글씨 이미지 높이/2 ) * (게임 내 글씨 크기 / 실제 글씨 이미지 높이)
    _float fTextBottomOffset = (m_fTextBottomPos_InPixel - m_fTextureHeight_InPixel * 0.5f) * (fTextHeight / m_fTextureHeight_InPixel);
	_float fShadowTopMargin = m_fShadowTopMargin_InPixel * (fTextHeight / m_fTextureHeight_InPixel);
    _float fShadowTopOffset = (m_fTextureHeight_InPixel * 0.5f - m_fShadowTopMargin_InPixel) * (fTextHeight / m_fTextureHeight_InPixel);

    _float fShadowOffsetZ = fTextBottomOffset  + fShadowTopOffset;

    m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, fBaseY , fToodeeZ - fShadowOffsetZ + m_fMotionPosition+ fPagePositionZ));
    m_pTransformCom->Scaling(fTextWidth, fTextHeight - m_fMotionSize, 1.f);
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(1)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();


    // 3. TOPDEE 그림자
    m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, fBaseY, fTopdeeZ + m_fMotionPosition+ fPagePositionZ));
    m_pTransformCom->Scaling(fTextWidth, fTextHeight+ m_fMotionSize, 1.f);
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(2)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();

    // 4. TOPDEE 텍스트 
    _float temp = fTextHeight - m_fMotionSize;
    if (temp < 10.f)
        temp =  10.f ;

    m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, fBaseY, fTopdeeZ -fShadowOffsetZ + m_fMotionPosition+ fPagePositionZ));
    m_pTransformCom->Scaling(fTextWidth, temp, 1.f);
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(3)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();

    // 5. AND 
    m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, fBaseY, fAndZ + m_fMotionPosition+ fPagePositionZ));
    m_pTransformCom->Scaling(16.f, 8.f, 1.f);
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(4)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();

	return S_OK;
}

void CMainMenu_Title::OnPageButtons(const MAINMENU_SWAP_EVENT& event)
{
    m_bIsMeinMenu = true; 
    m_bPageOnceChange = true;
    fEasedTime = 0.f;
}

void CMainMenu_Title::OnPageStage(const MAINMENU_SWAP_EVENT& event)
{
    m_bIsMeinMenu = false;
    m_bPageOnceChange = true;
    fEasedTime = 0.f;
}


void CMainMenu_Title::Change_Page(_float fTimeDelta)
{
    if (!m_bPageOnceChange) return;

    fEasedTime += fTimeDelta;

    if (m_bIsMeinMenu && EaseFloatInOutBack(&fPagePositionZ, fPageStagePositionZ, fPageButtonPositionZ, fEasedTime, 1.8f))
         m_bPageOnceChange = false;
    

    if (!m_bIsMeinMenu && EaseFloatInOutBack(&fPagePositionZ, fPageButtonPositionZ, fPageStagePositionZ, fEasedTime, 1.8f))
        m_bPageOnceChange = false;

}

HRESULT CMainMenu_Title::Ready_SubscribeEvent(_uint iPlayLevel)
{
    m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_BUTTON, [this](const MAINMENU_SWAP_EVENT& Event) {
        this->OnPageButtons(Event);
        });

    m_pGameInstance->Subscribe<MAINMENU_SWAP_EVENT>(iPlayLevel, EVENT_KEY::MAINMENU_STAGE, [this](const MAINMENU_SWAP_EVENT& Event) {
        this->OnPageStage(Event);
        });

    return S_OK;
}

HRESULT CMainMenu_Title::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Title"),
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

void CMainMenu_Title::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CMainMenu_Title::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CMainMenu_Title::Change_Motion(_float fTimeDelta)
{
    if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
    {
        m_fAccumulateMotionTime = 0.f;

        if (m_bMotionSize && m_fMotionSize >= 1.5f)
            m_bMotionSize = false;
        else if (!m_bMotionSize && m_fMotionSize <= -1.5f)
            m_bMotionSize = true;

		if (m_bMotionSize)   m_fMotionSize += fTimeDelta * 6.f;
        else  m_fMotionSize -= fTimeDelta * 6.f;

        if (m_bMotionPosition && m_fMotionPosition >= 1.f)
            m_bMotionPosition = false;
        else if (!m_bMotionPosition && m_fMotionPosition <= -1.f)
            m_bMotionPosition = true;

		if (m_bMotionPosition)   m_fMotionPosition += fTimeDelta * 5.f;
        else  m_fMotionPosition -= fTimeDelta * 5.f;


    }
    else
        m_fAccumulateMotionTime += fTimeDelta;
}

CMainMenu_Title* CMainMenu_Title::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu_Title* pInstance = new CMainMenu_Title(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainMenu_Title"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CMainMenu_Title::Clone(void* pArg)
{
	CMainMenu_Title* pInstance = new CMainMenu_Title(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMainMenu_Title"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu_Title::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
