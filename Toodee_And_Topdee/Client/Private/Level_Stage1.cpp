﻿#include "Level_Stage1.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "ColliderMap_Object.h"

#include "Test_Cube2.h"
#include "Cloud.h"
#include "BackRock.h"

CLevel_Stage1::CLevel_Stage1(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage1::Initialize()
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

	if (FAILED(Ready_Layer_Cloud(TEXT("Layer_Cloud"))))
		return E_FAIL;

	m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(TEXT("Stage1-2Bgm.ogg"), 0.5f);
	m_pGameInstance->Set_Active(TEXT("Effect_Wind"));
	return S_OK;
}

void CLevel_Stage1::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_STAGE2);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}
	if (m_pGameInstance->Key_Down(VK_ESCAPE))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_STAGE1);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_LOGO);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}

}

HRESULT CLevel_Stage1::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_Stage1"));

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera(const _wstring& strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/Stage1"));

	BLOCK_INFO	info = {};
	_uint		idx = {};

	while (S_OK == (m_pGameInstance->Get_Tile_Data(idx++, info)))
	{
		switch (static_cast<MAPOBJECT>(info.iBlockType))
		{
		case MAPOBJECT::NONE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), info.vPos)))
				return E_FAIL;
			break;
		case MAPOBJECT::WALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::WOOD:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallWood"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::BREAK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallBreak"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::LOCK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallLock"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::FALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallFall"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPARK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Spark"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::METAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Metal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::HOLE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::KEY:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Key"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::PORTAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOODEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Player_TooDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOPDEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Player_TopDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPIKE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Spikes"), &info)))
				return E_FAIL;
			break;
		default:
			MSG_BOX(TEXT("Error : Block Index error!"));
		}

	}

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 0;
	_uint BackTileIdx[2];

	BackTileIdx[0] = 32;
	BackTileIdx[1] = 18;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_BackCloud"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), &BackTileIdx)))
		return E_FAIL;

	for (_uint i = 0; i < 3; ++i)
	{
		CBackRock::BACK_DESC dsc;
		
		switch (i)
		{
		case 0 :
			dsc._pos = { -15.f, -.2f, -6.f };
			dsc._size = { 20.f, 20.f, 1.f };
			dsc._textureIdx = i;
			break;

		case 1:
			dsc._pos = { 15.f, -.3f, 13.f };
			dsc._size = { 12.f, 12.f, 1.f };
			dsc._textureIdx = i;
			break;

		case 2:
			dsc._pos = { 8.f, -.1f, 7.f };
			dsc._size = { 8.f, 17.f, 1.f };
			dsc._textureIdx = i;
			break;

		}
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_BackRock"), &dsc)))
			return E_FAIL;

		
	}
		

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_STAGE1); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_STAGE1, i);
		desc.vPosition = Pair.first;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Cloud(const _wstring& strLayerTag)
{
	CCloud::CLOUD_DESC desc;
	desc.eType = CCloud::CLOUD_TYPES::WHITE;
	desc.vPosition = { 25.f, 1.9f, 8.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE1), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cloud"), &desc)))
		return E_FAIL;
	return S_OK;
}

CLevel_Stage1* CLevel_Stage1::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage1* pInstance = new CLevel_Stage1(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	__super::Free();
}
