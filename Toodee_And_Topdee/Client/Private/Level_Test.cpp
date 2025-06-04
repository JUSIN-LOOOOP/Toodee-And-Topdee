#include "Level_Test.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"

#include "Test_Cube2.h"
#include "Pig.h"
#include "Collider_Cube.h"

CLevel_Test::CLevel_Test(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Test::Initialize()
{

	// if (FAILED(Ready_Observer()))
	// 	return E_FAIL;


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	// if (FAILED(Ready_Layer_TestCube(TEXT("Layer_TestCube"))))
	// 	return E_FAIL;

	// if (FAILED(Ready_Layer_TestCube2(TEXT("Layer_TestCube2"))))
	// 	return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Potal(TEXT("Layer_Potal"))))
	//	return E_FAIL;

//	if(FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
//		return E_FAIL;

//	if (FAILED(Ready_Layer_Tile(TEXT("Layer_Tiler"))))
//		return E_FAIL;

	if (FAILED(Ready_Layer_ColliderCube(TEXT("Layer_Cube"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_Test::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Test::Render()
{
	SetWindowText(g_hWnd, TEXT("CLevel_Test"));

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	/*CCamera::CAMERA_DESC			CameraDesc{};
	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Camera"), &CameraDesc)))
		return E_FAIL;*/

	CCamera::CAMERA_DESC			CameraDesc{};
	CameraDesc.vEye = _float3(0.f, 34.f, 0.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.1f);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.1f;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_TestCube(const _wstring& strLayerTag)
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
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallBlock"), &info)))
				return E_FAIL;
			break;
		case MAPOBJECT::WOOD:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallWood"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::BREAK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallBreak"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::LOCK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallLock"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::FALL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallFall"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPARK:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Spark"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::METAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Metal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::HOLE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Hole"), &info)))
				return E_FAIL;
			break;

		default :
			MSG_BOX(TEXT("Error : Block Index error!"));
		}
		
	}
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_TestCube2(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < 3; i++) 
		{
			CTest_Cube2::TEST_TRANS desc{};
			_float3 temp = { static_cast<_float>(i),0.f, 0.f };
			desc.Pos = temp;
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube2"), &desc)))
				return E_FAIL;
		}
		return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Tile(const _wstring& strLayerTag)
{
	_int MapHeight(18), MapWidth(32);
	for (_int height = 0; height < MapHeight; ++height)
	{
		for (_int width = 0; width < MapWidth; ++width)
		{
			_float3 pos{ (FLOAT)((width + 0.5 - (MapWidth / 2)) * 2) , 0.f,  (FLOAT)((height + 0.5 - (MapHeight / 2)) * 2) };

			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Tile"), &pos)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Potal(const _wstring& strLayerTag)
{
	_float3 vPotalPosition = { 5.f, 0.f, 0.f }; //TEST

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &vPotalPosition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_ColliderCube(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < 5; i++)
	{
		BLOCK_INFO Block_Desc{};
		Block_Desc.vPos = _float3(i * 2.f, 0.f, -5.f);
		Block_Desc.vScale = _float3(2.f, 2.f, 2.f);
		Block_Desc.iTextureIdx = 0;
		Block_Desc.iDir = 0;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_ColliderCube"), &Block_Desc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_Test::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CPig::PIG_DESC pDesc{};

	pDesc.TargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 1));
	pDesc.vPosSet = _float3(3.5f, 0.f, 10.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Pig"), &pDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Test* CLevel_Test::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Test* pInstance = new CLevel_Test(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Test::Free()
{
	__super::Free();
}
