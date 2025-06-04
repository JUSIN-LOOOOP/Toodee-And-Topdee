#include "Level_Stage2.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "ClearTriggerObserver.h"
#include "ColliderMap_Object.h"
#include "Test_Cube2.h"

CLevel_Stage2::CLevel_Stage2(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Stage2::Initialize()
{
	if (FAILED(Ready_Observer()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Potal(TEXT("Layer_Potal"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Back(TEXT("Layer_Background"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_ColliderMap(TEXT("Layer_ColliderMap"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Stage2::Update(_float fTimeDelta)
{
	// -- 임시 --
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_STAGE3))))
			return;
	}

}

HRESULT CLevel_Stage2::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_Stage2"));

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Camera(const _wstring& strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/Stage2"));

	BLOCK_INFO	info = {};
	_uint		idx = {};

	while (S_OK == (m_pGameInstance->Get_Tile_Data(idx++, info)))
	{
		switch (static_cast<MAPOBJECT>(info.iBlockType))
		{
		case MAPOBJECT::NONE:
			break;

		case MAPOBJECT::WALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::WOOD:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallWood"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::BREAK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallBreak"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::LOCK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallLock"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::FALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallFall"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPARK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Spark"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::METAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Metal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::HOLE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;
			break;

		default:
			MSG_BOX(TEXT("Error : Block Index error!"));
		}
	}
	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Potal(const _wstring& strLayerTag)
{
	_float3 vPotalPosition = { 5.f, 0.f, 0.f }; //TEST

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &vPotalPosition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 0;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_BackCloud"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Observer()
{
	// 옵저버 매니저에 Observer_ClearTrigger Key값을 가진 CClearTriggerObserver 생성
	if(FAILED(m_pGameInstance->Add_Observer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Observer_ClearTrigger"),
		CClearTriggerObserver::Create())))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_STAGE2); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_STAGE2, i);
		desc.vPosition = Pair.first;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGE2), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}


	return S_OK;
}

CLevel_Stage2* CLevel_Stage2::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Stage2* pInstance = new CLevel_Stage2(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Stage2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage2::Free()
{
	__super::Free();
}
