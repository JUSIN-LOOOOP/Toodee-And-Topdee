#include "SpeechBallon.h"
#include "GameInstance.h"

CSpeechBallon::CSpeechBallon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject { pGraphic_Device }
{
}

CSpeechBallon::CSpeechBallon(const CSpeechBallon& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CSpeechBallon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpeechBallon::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	SPEECH_DESC* pDesc = static_cast<SPEECH_DESC*>(pArg);

	m_strText = pDesc->strText;
	m_iTextLength = static_cast<_uint>(wcslen(m_strText));
	m_tRect = pDesc->tRect;
	m_dwColor = pDesc->dwColor;
	m_iSpeechNumber = pDesc->iSpeechNumber;

	m_iCurrentTextLength = 0;
	m_iCurrentSpeechNumber = 1;

	m_pGameInstance->Subscribe<SPEECHNUMBER_EVENT>(m_pGameInstance->Get_NextLevelID(), EVENT_KEY::SPEECH, [this](const SPEECHNUMBER_EVENT& Event) {
		this->SpeechNumber(Event);
		});

	m_fSizeX = m_tRect.right - m_tRect.left;
	m_fSizeY = m_tRect.bottom - m_tRect.top;
	m_fX = (m_tRect.left + m_tRect.right) * 0.5f;
	m_fY = (m_tRect.top + m_tRect.bottom) * 0.5f;

	m_pText = new _tchar[m_iTextLength + 1];

	return S_OK;
}

void CSpeechBallon::Priority_Update(_float fTimeDelta)
{
}

void CSpeechBallon::Update(_float fTimeDelta)
{

	if (m_iCurrentSpeechNumber != m_iSpeechNumber)
		return;

	m_fDealyTime += fTimeDelta;

	if (m_iSpeechNumber == 1 && m_fDealyTime < 3.f)
		return;

	m_fTime += fTimeDelta;

	if (m_fTime > 0.06f)
	{
		if(m_iCurrentTextLength < m_iTextLength)
			m_iCurrentTextLength++;
		
		m_fTime = 0.f;
	}

	if (m_iCurrentTextLength == m_iTextLength)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			SPEECHNUMBER_EVENT Event;
			Event.iCurrentSpeechNumber = m_iSpeechNumber + 1;
			m_pGameInstance->Publish(m_pGameInstance->Get_CurrentLevelID(), EVENT_KEY::SPEECH, Event);
		}
	}
	m_fSizeX = 60.f;
	m_fSizeX += m_iCurrentTextLength * 11.f;
}

void CSpeechBallon::Late_Update(_float fTimeDelta)
{
	if(m_iCurrentSpeechNumber == m_iSpeechNumber)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CSpeechBallon::Render()
{
	if (m_iSpeechNumber == 1 && m_fDealyTime < 3.f)
		return S_OK;

	if (FAILED(__super::Begin(m_pTransformCom)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();

	if (FAILED(__super::End()))
		return E_FAIL;


	wcsncpy_s(m_pText, m_iTextLength + 1, m_strText, m_iCurrentTextLength);

	m_pGameInstance->DrawFont(TEXT("Font_Arial"), m_pText, &m_tRect, m_dwColor);

	m_pText[0] = L'\0';

	return S_OK;
}

void CSpeechBallon::SpeechNumber(const SPEECHNUMBER_EVENT& Event)
{
	m_iCurrentSpeechNumber = Event.iCurrentSpeechNumber;
}

HRESULT CSpeechBallon::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_SpeechBallon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpeechBallon::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	return S_OK;
}

HRESULT CSpeechBallon::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

CSpeechBallon* CSpeechBallon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpeechBallon* pInstance = new CSpeechBallon(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CSpeechBallon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CSpeechBallon::Clone(void* pArg)
{
	CSpeechBallon* pInstance = new CSpeechBallon(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CSpeechBallon"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpeechBallon::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pText);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
