#include "Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"


CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
    :CLevel{pGraphicDev}
{
}

HRESULT CLevel_Logo::Initialize()
{
	/* 현재 레벨을 구성해주기 위한 객체들을 생성한다. */


    return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	// -- 임시 --
	if (m_pGameInstance->Key_Down(VK_RETURN))
	{
		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LEVEL_GAMEPLAY))))
			return;
	}

	return;
}

HRESULT CLevel_Logo::Render()
{
	// -- 임시 --
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
    __super::Free();
}
