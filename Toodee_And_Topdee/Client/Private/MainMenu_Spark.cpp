#include "MainMenu_Spark.h"
#include "GameInstance.h"

CMainMenu_Spark::CMainMenu_Spark(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CMainMenu_Spark::CMainMenu_Spark(const CMainMenu_Spark& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CMainMenu_Spark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainMenu_Spark::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	MAINMENU_SPARK* info = reinterpret_cast<MAINMENU_SPARK*>(pArg);
	m_iMotionMaxNum = info->iMotionMaxNum;
	m_vPosition = info->vPosition;
	if (m_vPosition.z < -10.f)
		m_iAlphaValue = 255;
	else if (m_vPosition.z < 0)
		m_iAlphaValue = 196;
	else if (m_vPosition.z < 10.f)
		m_iAlphaValue = 128;
	else if (m_vPosition.z < 20.f)
		m_iAlphaValue = 64;

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);

	return S_OK;
}

void CMainMenu_Spark::Priority_Update(_float fTimeDelta)
{
}

void CMainMenu_Spark::Update(_float fTimeDelta)
{
	Change_Motion(fTimeDelta);
}

void CMainMenu_Spark::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CMainMenu_Spark::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iMotionNum)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	return S_OK;
}

void CMainMenu_Spark::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
	{
		m_fAccumulateMotionTime = 0.f;
		++m_iMotionNum;
		m_iMotionNum = m_iMotionNum == m_iMotionMaxNum + 1? 0 : m_iMotionNum;
	}
	else
		m_fAccumulateMotionTime += fTimeDelta;
}

HRESULT CMainMenu_Spark::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Spark"),
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

void CMainMenu_Spark::SetUp_RenderState()
{
	// 알파 블렌딩 설정
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 연하게 만들기 위한 텍스처팩터 설정 (알파값 조정)
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlphaValue, 255, 255, 255)); // 50% 투명

	// 텍스처 색상과 팩터를 곱해서 연하게 만들기
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	// 알파 값도 텍스처와 팩터를 곱해서 부드럽게
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

}

void CMainMenu_Spark::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}


CMainMenu_Spark* CMainMenu_Spark::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMainMenu_Spark* pInstance = new CMainMenu_Spark(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainMenu_Spark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CMainMenu_Spark::Clone(void* pArg)
{
	CMainMenu_Spark* pInstance = new CMainMenu_Spark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMainMenu_Spark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainMenu_Spark::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
