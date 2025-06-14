#include "Level_MapEdit.h"
#include "GameInstance.h"
#include "Camera.h"
#include "BasicTile.h"
#include "Level_Loading.h"
#include "TextureUI.h"

CLevel_Map::CLevel_Map(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Map::Initialize()
{
	vector<_uint> TextureCount;

	TextureCount.push_back(1);	//NONe
	TextureCount.push_back(99);	//WALL
	TextureCount.push_back(1);	//WOOD
	TextureCount.push_back(1);	//BREAK
	TextureCount.push_back(1);	//LOCK
	TextureCount.push_back(1);	//FALL
	TextureCount.push_back(1);	//SPARK
	TextureCount.push_back(1);	//METAL
	TextureCount.push_back(1);	//Hole
	TextureCount.push_back(1);	//Key
	TextureCount.push_back(1);	//Potal
	TextureCount.push_back(1);	//Topdee
	TextureCount.push_back(1);	//Toodee
	TextureCount.push_back(1);	//Spike

	m_pGameInstance->Load_Initial_Data(&TextureCount);

    if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Tile(TEXT("Layer_Tile"))))
        return E_FAIL;

	if (FAILED(Ready_Layer_TextureUI(TEXT("Layer_Texture"))))
		return E_FAIL;

    return S_OK;
}

void CLevel_Map::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Map::Render()
{
    SetWindowText(g_hWnd, TEXT("맵 에디팅"));

    return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};
	CameraDesc.vEye = _float3(0.f, 32.f, 0.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.1f );
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.3f;


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Tile(const _wstring& strLayerTag)
{
	m_pGameInstance->Load_File(TEXT("../Resources/Map/StageBoss"));
	//m_pGameInstance->Load_File(TEXT("new"));
	BLOCK_INFO	info = {};
	BLOCK_INFO	arg = {};
	_uint		idx = {};

	_int MapHeight(18), MapWidth(32);
	//_int MapHeight(14), MapWidth(116);
	for (_int height = 0; height < MapHeight; ++height)
	{
		for (_int width = 0; width < MapWidth; ++width)
		{
			m_pGameInstance->Get_Tile_Data(idx++, info);
			_float3 pos{ (FLOAT)((width + 0.5 - (MapWidth / 2)) * 2) , 0.f,  (FLOAT)((height + 0.5 - (MapHeight / 2)) * 2) };
			arg.iBlockType = info.iBlockType;
			arg.iDir = info.iDir;
			arg.iTextureIdx = info.iTextureIdx;
			arg.iTileTextureIdx = info.iTileTextureIdx;
			arg.vPos = pos;
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Tile"), &arg)))
				return E_FAIL;
			arg = {};
		}
	}
	
	return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_TextureUI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_TextureUI"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Map* CLevel_Map::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Map* pInstance = new CLevel_Map(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Map"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Map::Free()
{
	__super::Free();

}
