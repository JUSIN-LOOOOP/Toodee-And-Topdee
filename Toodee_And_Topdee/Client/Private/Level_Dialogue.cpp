#include "Level_Dialogue.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "ColliderMap_Object.h"

#include "Test_Cube2.h"
#include "Cloud.h"
#include "BackRock.h"
#include "SpeechBallon.h"

CLevel_Dialogue::CLevel_Dialogue(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Dialogue::Initialize()
{
	m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);
	m_pGameInstance->Reset_KeyCount();
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Back(TEXT("Layer_Background"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ColliderMap(TEXT("Layer_ColliderMap"))))
		return E_FAIL;

	if (FAILED(Ready_Speech(TEXT("Layer_Speech"))))
		return E_FAIL;

	m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(TEXT("DialogueBgm.ogg"), 0.5f);	//수정

	return S_OK;
}

void CLevel_Dialogue::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_DIALOGUE);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}
	if (m_pGameInstance->Key_Down(VK_ESCAPE))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_DIALOGUE);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_LOGO);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}

}

HRESULT CLevel_Dialogue::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_Dialogue"));

	return S_OK;
}

HRESULT CLevel_Dialogue::Ready_Layer_Camera(const _wstring& strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Dialogue::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/Dialogue"));

	BLOCK_INFO	info = {};
	_uint		idx = {};

	while (S_OK == (m_pGameInstance->Get_Tile_Data(idx++, info)))
	{
		switch (static_cast<MAPOBJECT>(info.iBlockType))
		{
		case MAPOBJECT::NONE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), info.vPos)))
				return E_FAIL;
			break;

		case MAPOBJECT::WALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::PORTAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOODEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), TEXT("Player_TooDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOPDEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), TEXT("Player_TopDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"), &info)))
				return E_FAIL;
			break;
		}

	}

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), TEXT("Small_Toodoo"),
		ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), TEXT("Prototype_GameObject_SmallToodd"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Dialogue::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 4;
	_uint BackTileIdx[2];

	BackTileIdx[0] = 116;
	BackTileIdx[1] = 14;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), &BackTileIdx)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Dialogue::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_FINALBOSS2); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_FINALBOSS2, i);
		desc.vPosition = Pair.first;
		desc.vPosition.z += (desc.vPosition.z < 0) ? 2.f : -2.f;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}


	return S_OK;
}
//730
HRESULT CLevel_Dialogue::Ready_Speech(const _wstring& strLayerTag)
{
	CSpeechBallon::SPEECH_DESC Desc{};
	Desc.strText = TEXT("It is quite a surprise that you got this far");
	Desc.tRect = RECT{ 400, 150, 950, 260 };
	Desc.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	Desc.iSpeechNumber = 1;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_SpeechBallon"), &Desc)))
		return E_FAIL;

	Desc.strText = TEXT("Each one of you, is tiny and weak");
	Desc.tRect = RECT{ 420, 150, 950, 260 };
	Desc.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	Desc.iSpeechNumber = 2;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_SpeechBallon"), &Desc)))
		return E_FAIL;

	Desc.strText = TEXT("But i didn’t expect such a threat");
	Desc.tRect = RECT{ 420, 150, 950, 260 };
	Desc.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	Desc.iSpeechNumber = 3;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_SpeechBallon"), &Desc)))
		return E_FAIL;


	Desc.strText = TEXT("So I’ve planed a courrpution");
	Desc.tRect = RECT{ 420, 150, 950, 260 };
	Desc.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	Desc.iSpeechNumber = 4;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_SpeechBallon"), &Desc)))
		return E_FAIL;

	Desc.strText = TEXT("Now it’s going to expand and swallow everything here");
	Desc.tRect = RECT{ 380, 150, 970, 260 };
	Desc.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	Desc.iSpeechNumber = 5;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_SpeechBallon"), &Desc)))
		return E_FAIL;


	Desc.strText = TEXT("Welcome to “THRID DIMENSION ”");
	Desc.tRect = RECT{ 420, 150, 950, 260 };
	Desc.dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	Desc.iSpeechNumber = 6;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_SpeechBallon"), &Desc)))
		return E_FAIL;



	return S_OK;
}

CLevel_Dialogue* CLevel_Dialogue::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Dialogue* pInstance = new CLevel_Dialogue(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Dialogue"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Dialogue::Free()
{
	__super::Free();
}
