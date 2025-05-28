#include "Level_MapEdit.h"
#include "GameInstance.h"
#include "Camera.h"
#include "BasicTile.h"
#include "Level_Loading.h"

CLevel_Map::CLevel_Map(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Map::Initialize()
{
	vector<_uint> TextureCount;

	TextureCount.push_back(1);
	TextureCount.push_back(ENUM_CLASS(WALLTYPE::WALL_EMD));
	TextureCount.push_back(ENUM_CLASS(WOODTEXTURE::WOOD_EMD));
	TextureCount.push_back(ENUM_CLASS(SPIKETEXTURE::SPIKE_EMD));



	m_pGameInstance->Load_Initial_Data(&TextureCount);

    if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Tile(TEXT("Layer_Tile"))))
        return E_FAIL;

	/*if (FAILED(Ready_Layer_TextureUI(TEXT("Layer_Texture"))))
		return E_FAIL;*/

    return S_OK;
}

void CLevel_Map::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Map::Render()
{
    SetWindowText(g_hWnd, TEXT("¸Ê ¿¡µðÆÃ"));

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
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_MultiViewCamera"), &CameraDesc)))
		return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Map::Ready_Layer_Tile(const _wstring& strLayerTag)
{
	_int MapHeight(18), MapWidth(32);
	for (_int height = 0; height < MapHeight; ++height)
	{
		for (_int width = 0; width < MapWidth; ++width)
		{
			_float3 pos{ (FLOAT)((width + 0.5 - (MapWidth / 2)) * 2) , 0.f,  (FLOAT)((height + 0.5 - (MapHeight / 2)) * 2) };
			
			if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), strLayerTag,
				ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Tile"), &pos)))
				return E_FAIL;
		}
	}
	

	return S_OK;
}

//HRESULT CLevel_Map::Ready_Layer_TextureUI(const _wstring& strLayerTag)
//{
//	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), strLayerTag,
//		ENUM_CLASS(LEVEL::LEVEL_MAP), TEXT("Prototype_GameObject_UI"))))
//		return E_FAIL;
//
//	return S_OK;
//}

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
