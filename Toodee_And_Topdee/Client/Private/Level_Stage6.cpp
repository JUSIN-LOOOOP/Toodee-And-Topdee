#include "Level_Stage6.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "ColliderMap_Object.h"

#include "Test_Cube2.h"
#include "Fire_Projectile.h"
#include "Cloud.h"

CLevel_Stage6::CLevel_Stage6(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage6::Initialize()
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

	if (FAILED(Ready_Layer_Cannon(TEXT("Layer_Cannon"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_DarkCloud(TEXT("Layer_DarkCloud"))))
		return E_FAIL;
	
	return S_OK;
}

void CLevel_Stage6::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Stage6::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_Stage6"));

	return S_OK;
}

HRESULT CLevel_Stage6::Ready_Layer_Camera(const _wstring& strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage6::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/Stage6"));

	BLOCK_INFO	info = {};
	_uint		idx = {};

	while (S_OK == (m_pGameInstance->Get_Tile_Data(idx++, info)))
	{
		switch (static_cast<MAPOBJECT>(info.iBlockType))
		{
		case MAPOBJECT::NONE:
			break;

		case MAPOBJECT::WALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::WOOD:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_WallWood"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::BREAK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_WallBreak"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::LOCK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_WallLock"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::FALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_WallFall"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPARK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Spark"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::METAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Metal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::HOLE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::KEY:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Key"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::PORTAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOODEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOPDEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"), &info)))
				return E_FAIL;
			break;

		default:
			MSG_BOX(TEXT("Error : Block Index error!"));
		}

	}
	return S_OK;
}

HRESULT CLevel_Stage6::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 2;
	_uint BackWallThemeIdx = 1;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackWall"), &BackWallThemeIdx)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage6::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_STAGE6); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_STAGE6, i);
		desc.vPosition = Pair.first;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Stage6::Ready_Layer_Cannon(const _wstring& strLayerTag)
{
	/* Prototype_GameObject_Cannon */
	CCannon::CANNON_INFO info1{};
	info1.eDir = CCannon::CANNON_DIRECTION::RGIHT;
	info1.eType = CCannon::CANNON_TYPE::FIRE;
	info1.vPosition = { -32.f,2.f,2.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cannon"), &info1)))
		return E_FAIL;


	CCannon::CANNON_INFO info2{};
	info2.eDir = CCannon::CANNON_DIRECTION::RGIHT;
	info2.eType = CCannon::CANNON_TYPE::FIRE;
	info2.vPosition = { -32.f,2.f,-6.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cannon"), &info2)))
		return E_FAIL;

	CCannon::CANNON_INFO info3{};
	info3.eDir = CCannon::CANNON_DIRECTION::RGIHT;
	info3.eType = CCannon::CANNON_TYPE::FIRE;
	info3.vPosition = { -32.f,2.f,-14.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cannon"), &info3)))
		return E_FAIL;




	/* Prototype_GameObject_Projectile */
	for (_uint i = 0; i < 50; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Layer_Projectile_Fire"),
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Projectile_Fire"))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Stage6::Ready_Layer_DarkCloud(const _wstring& strLayerTag)
{
	CCloud::CLOUD_DESC desc;
	desc.eType = CCloud::CLOUD_TYPES::DARK;
	desc.vPosition = { 25.f, 1.9f, 6.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cloud"), &desc)))
		return E_FAIL;

	for (_uint i = 0; i < 40; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Layer_Rain"),
			ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Rain"))))
			return E_FAIL;
	}

	for (_uint i = 0; i < 4; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Layer_RainSplash"),
			ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_RainSplash"))))
			return E_FAIL;
	}

	for (_uint i = 0; i < 4; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Layer_Lightning"),
			ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Lightning"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Layer_Storm"),
		ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Storm"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Stage6* CLevel_Stage6::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage6* pInstance = new CLevel_Stage6(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage6"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage6::Free()
{
	__super::Free();
}
