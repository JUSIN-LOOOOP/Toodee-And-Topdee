#include "Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera.h"

#include "MainMenu_Spark.h"


CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
    :CLevel{pGraphicDev}
{
}

HRESULT CLevel_Logo::Initialize()
{
	/* 현재 레벨을 구성해주기 위한 객체들을 생성한다. */
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Spark(TEXT("Layer_Spark"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_RotationObject(TEXT("Layer_RotationObject"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Buttons(TEXT("Layer_Buttons"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Title(TEXT("Layer_Title"))))
		return E_FAIL;

    return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	// -- 임시 --
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_GAMEPLAY))))
			return;
	}

	return;
}

HRESULT CLevel_Logo::Render()
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};
	CameraDesc.vEye = _float3(0.f, 34.f, 0.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.1f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_BackGround"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Spark(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < 18; i++)
	{
		CMainMenu_Spark::MAINMENU_SPARK info;
		info.iMotionMaxNum = static_cast<_uint>( m_pGameInstance->Rand(0.f, 10.f));
		info.vPosition = _float3(m_pGameInstance->Rand(-31.f, 31.f), 1.f, m_pGameInstance->Rand(-17.f, 17.f));

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_LOGO), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_Spark"),&info)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_RotationObject(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_CenterLine"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Buttons(const _wstring& strLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Title(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_LOGO), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_Title"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
    __super::Free();
}
