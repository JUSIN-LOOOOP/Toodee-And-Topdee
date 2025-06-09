#include "Level_GamePlay.h"
#include "Client_Extension.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Level_MapEdit.h"
#include "Level_Loading.h"
#include "Test_Cube2.h"
#include "Cannon.h"
#include "Pig.h"
#include "ColliderMap_Object.h"
#include "Cloud.h"
#include "Storm.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_TestCube(TEXT("Layer_TestCube"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_TestCube2(TEXT("Layer_TestCube2"))))
	//	return E_FAIL;

	/*if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Back(TEXT("Layer_Back"))))
		return E_FAIL;

	//if(FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Tile(TEXT("Layer_Tiler"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Cannon(TEXT("Layer_Cannon"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_ColliderMap(TEXT("Layer_ColliderMap"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Cloud(TEXT("Layer_Cloud"))))
	//	return E_FAIL;

	/*if (FAILED(Ready_Layer_StageBoss(TEXT("Layer_StageMonster"))))
		return E_FAIL;*/

	//if (FAILED(Ready_Layer_Block_Disappear(TEXT("Layer_Block_Disappear"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_RedButton(TEXT("Layer_RedButton"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	// -- 임시 --
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_STAGE1))))
			return;
	}

}

HRESULT CLevel_GamePlay::Render()
{
	//SetWindowText(g_hWnd, TEXT("CLevel_GamePlay"));
	m_pGameInstance->View_FrameRate(g_hWnd);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_TestCube(const _wstring& strLayerTag)
{
	//m_pGameInstance->Load_File(TEXT("new"));
	m_pGameInstance->Load_File(TEXT("../Resources/Map/Stage1"));
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
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info)))
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

		case MAPOBJECT::KEY:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Key"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::PORTAL:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOODEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Player_TooDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::TOPDEE:
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Player_TopDee"),
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"), &info)))
				return E_FAIL;
			break;

		case MAPOBJECT::SPIKE:
			/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Spike"), &info)))
				return E_FAIL;*/
			break;

		default:
			MSG_BOX(TEXT("Error : Block Index error!"));
		}

	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_TestCube2(const _wstring& strLayerTag)
{
	//CTest_Cube2::TEST_TRANS desc{};
	//_float3 temp = { 1.f,0.f, 1.f };
	//desc.Pos = temp;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube2"), &desc)))
	//	return E_FAIL;

	//CTest_Cube2::TEST_TRANS desc2{};
	//_float3 temp2 = { 3.f,0.f, 3.f };
	//desc2.Pos = temp2;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube2"), &desc2)))
	//	return E_FAIL;

	//CTest_Cube2::TEST_TRANS desc3{};
	//_float3 temp3 = { 3.f,0.f, 1.f };
	//desc3.Pos = temp3;
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
	//	ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube2"), &desc3)))
	//	return E_FAIL;

	BLOCK_INFO info = {
		_float3(3.f, 1.f, 1.f),
		_float3(2.f,2.f,2.f),
		0,
		0,
		0,
		0
	};

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Block_Wood"), &info)))
		return E_FAIL;

	BLOCK_INFO info1 = {
	_float3(5.f, 1.f, 1.f),
	_float3(2.f,2.f,2.f),
	0,
	0,
	0,
	0
	};

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Block_Wood"), &info1)))
		return E_FAIL;

	BLOCK_INFO info2 = {
_float3(9.f, 1.f, 1.f),
_float3(2.f,2.f,2.f),
0,
0,
0,
0
	};

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"), &info2)))
		return E_FAIL;

	BLOCK_INFO info3 = {
_float3(1.f, 1.f, 1.f),
_float3(2.f,2.f,2.f),
0,
0,
0,
0

	};

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Block_Wood"), &info3)))
		return E_FAIL;

	return S_OK;
}

/*
HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"))))
		return E_FAIL;*/

		/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Spikes"))))
			return E_FAIL;


		BLOCK_INFO info = {
			_float3(3.f, 0.f, 3.f),
			_float3(2.f,2.f,2.f),
			0,
			0,
			0,
			0
		};
		if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallFall"),&info)))
			return E_FAIL;

		return S_OK;
	}

	HRESULT CLevel_GamePlay::Ready_Layer_Potal(const _wstring& strLayerTag)
	{
		_float3 vPotalPosition = { 5.f, 0.f, 0.f }; //TEST

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"), &vPotalPosition)))
			return E_FAIL;

		return S_OK;
	}
	*/

HRESULT CLevel_GamePlay::Ready_Layer_Back(const _wstring& strLayerTag)
{
	_uint BackdropThemeIdx = 0;
	_uint BackWallThemeIdx = 1;
	_uint BackTileThemeIdx[2];
	BackTileThemeIdx[0] = 32;
	BackTileThemeIdx[1] = 18;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackWall"), &BackWallThemeIdx)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"), BackTileThemeIdx)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_BackCloud"))))
		return E_FAIL;

	return S_OK;
}



HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CPig::PIG_DESC pDesc{};

	pDesc.TargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform"), 1));
	pDesc.vPosSet = _float3(3.5f, 0.f, 10.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Pig"), &pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_ColliderMap(const _wstring& strLayerTag)
{
	CColliderMap_Object::COLLIDER_MAP_DESC desc{};

	for (_uint i = 0; i < Stage_ColliderCount(LEVEL::LEVEL_GAMEPLAY); ++i)
	{
		auto Pair = MapCollider_Builder(LEVEL::LEVEL_GAMEPLAY, i);
		desc.vPosition = Pair.first;
		desc.vScale = Pair.second;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
			ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"), &desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Cloud(const _wstring& strLayerTag)
{

	CCloud::CLOUD_DESC desc;
	desc.eType = CCloud::CLOUD_TYPES::WHITE;
	desc.vPosition = { 25.f, 1.9f, 8.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Cloud"), &desc)))
		return E_FAIL;

	for (_uint i = 0; i < 40; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Rain"),
			ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Rain"))))
			return E_FAIL;
	}

	for (_uint i = 0; i < 4; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_RainSplash"),
			ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_RainSplash"))))
			return E_FAIL;
	}

	for (_uint i = 0; i < 4; ++i) {
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Lightning"),
			ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Lightning"))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Storm"),
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Storm"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Block_Disappear(const _wstring& strLayerTag)
{
	_float3 pos = { -10.f,8.f,0.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY),strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Block_Disappear"), &pos )))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_RedButton(const _wstring& strLayerTag)
{
	_float3 pos = { 12.f,1.f,4.f };

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_RedButton"), &pos)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_StageBoss(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_StageBoss"))))
		return E_FAIL;
	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
