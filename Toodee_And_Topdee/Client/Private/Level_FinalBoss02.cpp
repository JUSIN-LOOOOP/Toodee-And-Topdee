#include "Level_FinalBoss02.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "ColliderMap_Object.h"

#include "Test_Cube2.h"
#include "Event.h"
#include "MainMenu_Spark.h"

CLevel_FinalBoss02::CLevel_FinalBoss02(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_FinalBoss02::Initialize()
{
	m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);
	m_pGameInstance->Reset_KeyCount();

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Back(TEXT("Layer_Background"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ColliderMap(TEXT("Layer_ColliderMap"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_FinalBoss02::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3);
	
		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}
	if (m_pGameInstance->Key_Down(VK_ESCAPE))
	{
		LEVELCHANGE_EVENT Event;
		Event.iCurrentLevel = ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2);
		Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_LOGO);

		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
	}

	m_fIdleTime += fTimeDelta;

	if (m_fIdleTime >= 8.f && m_bAtkFlag[SEMICLON] == false && m_bAtkFlag[FINGER] == false)
		m_bAtkFlag[rand() % 2] = true;

	if (m_bAtkFlag[SEMICLON] == true || m_bAtkFlag[FINGER] == true)
		CreateHitBox(fTimeDelta);
}

HRESULT CLevel_FinalBoss02::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_FinalBoss02"));

	return S_OK;
}

HRESULT CLevel_FinalBoss02::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(-120.f, 8.f, 0.f);
	CameraDesc.vAt = _float3(120.f, -70.f, 0.1f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = 1.f; // D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_FPSCam"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_FinalBoss02::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/FinalBoss2"));

	BLOCK_INFO	info = {};
	_uint		idx = {};

	while (S_OK == (m_pGameInstance->Get_Tile_Data(idx++, info)))
	{
		switch (static_cast<MAPOBJECT>(info.iBlockType))
		{
		case MAPOBJECT::NONE:
			break;

		case MAPOBJECT::WALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::WOOD:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_WallWood"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::BREAK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_WallBreak"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::LOCK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_WallLock"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::FALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_WallFall"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPARK:
			/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Spark"), &info)))
				return E_FAIL;*/
			//-> 2D Spike
			break;

		case MAPOBJECT::METAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Metal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::HOLE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;
			/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;*/
			//-> Add another object here, only for rendering
			break;

		case MAPOBJECT::KEY:
			/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Key"), &info)))
				return E_FAIL;*/
			//-> [ Level_FinalBoss02 ] Cannon or laser position
			break;

		case MAPOBJECT::PORTAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOODEE:
			/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"), &info)))
				return E_FAIL;*/
			//이것도 임시!
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Player_TopDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Thirdee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOPDEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::SPIKE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Spikes"), &info)))
				return E_FAIL;
			break;
		default:
			MSG_BOX(TEXT("Error : Block Index error!"));
		}

	}
	return S_OK;
}

HRESULT CLevel_FinalBoss02::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 3;
	_uint BackTileIdx[2];

	BackTileIdx[0] = 116;
	BackTileIdx[1] = 14;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), &BackTileIdx)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_FinalBoss"))))
		return E_FAIL;
	for (_uint i = 0; i < 300; i++)
	{
		CMainMenu_Spark::MAINMENU_SPARK info;
		info.iMotionMaxNum = static_cast<_uint>(m_pGameInstance->Rand(0.f, 10.f));
		info.vPosition = _float3(m_pGameInstance->Rand(-200.f, 200.f), m_pGameInstance->Rand(-8.f, 18.f), m_pGameInstance->Rand(-17.f, 17.f));
		info.bFinalBoss = true;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MainMenu_Spark"), &info)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_FinalBoss02::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_FINALBOSS2); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_FINALBOSS2, i);
		desc.vPosition = Pair.first;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}


	return S_OK;
}

void CLevel_FinalBoss02::CreateHitBox(_float fTimeDelta)
{
	static _uint	Count = 0;
	_float pos[12] = { -11.f, -9.f, -7.f, -5.f, -3.f, -1.f, 1.f, 3.f, 5.f, 7.f, 9.f, 11.f };

	m_fDelayTime += fTimeDelta;

	if (m_fDelayTime >= 1.f)
	{
		FIANLBOSSATTACK_EVENT event;

		_float3 vPos = { 115.f, 8.f, (_float)pos[rand() % 12] };
		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Layer_HitBox"),
			ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), (m_bAtkFlag[SEMICLON] == true) ? TEXT("Prototype_GameObject_Semiclon") : TEXT("Prototype_GameObject_Finger"), &vPos);
		m_fDelayTime = 0;
		++Count;

		if (Count == 5)
		{
			(m_bAtkFlag[SEMICLON] == true) ? m_bAtkFlag[SEMICLON] = false : m_bAtkFlag[FINGER] = false;
			m_fIdleTime = 0;
			Count = 0;
			event.bIsAttacking = false;
		}
		else
		{
			event.bIsAttacking = true;

		}
		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::FINALBOSS_ATTACK, event);
	}
	
}

CLevel_FinalBoss02* CLevel_FinalBoss02::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_FinalBoss02* pInstance = new CLevel_FinalBoss02(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_FinalBoss02"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_FinalBoss02::Free()
{
	__super::Free();
}
