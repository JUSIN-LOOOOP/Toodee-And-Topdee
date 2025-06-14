#include "Level_Ending.h"
#include "GameInstance.h"
#include "Camera.h"

CLevel_Ending::CLevel_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CLevel {pGraphic_Device}
{
}

HRESULT CLevel_Ending::Initialize()
{
    if (FAILED(Ready_Layer_Back(TEXT("Layer_Background"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
        return E_FAIL;
    if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
        return E_FAIL;


    return S_OK;
}

void CLevel_Ending::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Ending::Render()
{
    SetWindowText(g_hWnd, TEXT("CLevel_Ending"));

    return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Camera(const _wstring& strLayerTag)
{
    CCamera::CAMERA_DESC			CameraDesc{};
    CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
    CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
    CameraDesc.fFovy = D3DXToRadian(60.0f);
    CameraDesc.fNear = 0.1f;
    CameraDesc.fFar = 1000.f;
    CameraDesc.fSpeedPerSec = 10.f;
    CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
    CameraDesc.fMouseSensor = 0.3f;

    if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_ENDING), strLayerTag,
        ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_GameObject_Camera"), &CameraDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_MapObject(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_ENDING), strLayerTag,
        ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_GameObject_Thirdee_Ending"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_ENDING), strLayerTag,
        ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_GameObject_Toodoo_Ending"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_ENDING), strLayerTag,
        ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_GameObject_Ending_Flash"))))
        return E_FAIL;
    
    return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_Back(const _wstring& strLayerTag)
{
    _uint BackdropThemeIdx = 3;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_ENDING), strLayerTag,
        ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"), &BackdropThemeIdx)))
        return E_FAIL;

    return S_OK;
}

CLevel_Ending* CLevel_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLevel_Ending* pInstance = new CLevel_Ending(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed to Created : CLevel_Ending"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Ending::Free()
{
    __super::Free();
}
