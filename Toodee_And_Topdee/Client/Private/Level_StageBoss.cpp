﻿#include "Level_StageBoss.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "ColliderMap_Object.h"

#include "Test_Cube2.h"
#include "Cannon.h"

CLevel_StageBoss::CLevel_StageBoss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_StageBoss::Initialize()
{
	m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);
	m_pGameInstance->Reset_KeyCount();
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Back(TEXT("Layer_Background"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Cannon(TEXT("Layer_Cannon"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ColliderMap(TEXT("Layer_ColliderMap"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_StageBoss(TEXT("Layer_StageMonster"))))
		return E_FAIL;

	Ready_SubscribeEvent(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS));
	m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(TEXT("StageBossBgm.ogg"), 0.2f);
	m_pGameInstance->Set_Active(TEXT("Effect_FireFly"));
	m_pGameInstance->Set_Active(TEXT("Effect_ColorLight"));
	m_pGameInstance->Set_Active(TEXT("Effect_CloudEffect"));

	return S_OK;
}

void CLevel_StageBoss::Update(_float fTimeDelta)
{
	// -- 임시 --

	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_STAGE4);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}
	if (m_pGameInstance->Key_Down(VK_ESCAPE))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_LOGO);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}
}

HRESULT CLevel_StageBoss::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_StageBoss"));

	return S_OK;
}

void CLevel_StageBoss::ResetBlock(const FIANLBOSSRESET_EVENT& Event)
{

	++ m_DeadCount;

	if (m_DeadCount == 2)
	{
		REMOVE_SPIKE tmp;
		BLOCK_INFO info;
		info.vPos = { 0.f,1.f,- 10.f };
		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), EVENT_KEY::REMOVE_SPIKE, tmp);
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Layer_MapObject"),
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info);
		m_pGameInstance->Set_Active(TEXT("Effect_PotalEffect"));
		for (_uint i = 0; i < 4; ++i)
			m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Layer_MapObject"), ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallLock"), &m_LockBlockInfo[i]);
		return;
	}

	m_pGameInstance->Reset_KeyCount();
	for (_uint i = 0; i < 3; ++i)
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Layer_MapObject"), ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Key"), &m_KeyInfo[i]);

	for (_uint i = 0; i < 4; ++i)
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Layer_MapObject"), ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallLock"), &m_LockBlockInfo[i]);
	
	for (_uint i = 0; i < 30; ++i)
	{
		BLOCK_INFO info = m_SpikeInfo[i];
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Layer_Spike"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Spikes"), &info);
		m_pGameInstance->Set_Active(TEXT("Effect_BlockDust"), &info);
	}
}

HRESULT CLevel_StageBoss::Ready_SubscribeEvent(_uint Level)
{
	m_pGameInstance->Subscribe<FIANLBOSSRESET_EVENT>(Level, EVENT_KEY::STAGEBOSS_RESET, [this](const FIANLBOSSRESET_EVENT& Event) {
		this->ResetBlock(Event);
		});
	return S_OK;
}

HRESULT CLevel_StageBoss::Ready_Layer_Camera(const _wstring& strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_StageBoss::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/StageBoss"));

	BLOCK_INFO	info = {};
	_uint		idx = {};
	_uint		LockBlockIdx = {};
	_uint		KeyIdx = {};
	_uint		SpikeIdx = {};

	while (S_OK == (m_pGameInstance->Get_Tile_Data(idx++, info)))
	{
		switch (static_cast<MAPOBJECT>(info.iBlockType))
		{
		case MAPOBJECT::NONE:
			break;

		case MAPOBJECT::WALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::WOOD:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallWood"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::BREAK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallBreak"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::LOCK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallLock"), &info)))
				return E_FAIL;
			m_LockBlockInfo[LockBlockIdx++] = info;
			break;

		case MAPOBJECT::FALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallFall"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPARK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Spark"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::METAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Metal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::HOLE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::KEY:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Key"), &info)))
				return E_FAIL;
			m_KeyInfo[KeyIdx++] = info;
			break;

		case MAPOBJECT::PORTAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOODEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Player_TooDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOPDEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Player_TopDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::SPIKE:
			m_SpikeInfo[SpikeIdx++] = info;
			break;
		default:
			MSG_BOX(TEXT("Error : Block Index error!"));
		}

	}
	return S_OK;
}

HRESULT CLevel_StageBoss::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 2;
	_uint BackWallThemeIdx = 2;
	_uint BackTileIdx[2];

	BackTileIdx[0] = 32;
	BackTileIdx[1] = 18;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), &BackTileIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackWall"), &BackWallThemeIdx)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_StageBoss::Ready_Layer_Cannon(const _wstring& strLayerTag)
{
	CCannon::CANNON_INFO info1{};
	info1.eDir = CCannon::CANNON_DIRECTION::LEFT;
	info1.eType = CCannon::CANNON_TYPE::FIRE;
	info1.vPosition = { 30.f,1.f,0.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cannon"), &info1)))
		return E_FAIL;

	/* Prototype_GameObject_Projectile */
	for (_uint i = 0; i < 15; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Layer_Projectile_Fire"),
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Projectile_Fire"))))
			return E_FAIL;
	}

	return S_OK;

}


HRESULT CLevel_StageBoss::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_STAGEBOSS); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_STAGEBOSS, i);
		desc.vPosition = Pair.first;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_StageBoss::Ready_Layer_StageBoss(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_StageBoss"))))
		return E_FAIL;
	return S_OK;
}

CLevel_StageBoss* CLevel_StageBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_StageBoss* pInstance = new CLevel_StageBoss(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_StageBoss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_StageBoss::Free()
{
	__super::Free();
}
