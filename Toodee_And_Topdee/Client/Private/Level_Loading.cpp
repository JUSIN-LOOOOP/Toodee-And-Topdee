#include "Level_Loading.h"
#include "GameInstance.h"
#include "Loader.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_MapEdit.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"
#include "Level_Stage3.h"
#include "Level_Stage4.h"
#include "Level_Stage5.h"
#include "Level_Stage6.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphicDev)
	:CLevel{ pGraphicDev }
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	if (FAILED(Ready_GameObjects()))
		return E_FAIL;

	if (FAILED(Ready_LoadingThread()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->IsFinished() && m_pGameInstance->Key_Down(VK_SPACE))
	{
		CLevel* pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL::LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_GAMEPLAY:
			pNewLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE1:
			pNewLevel = CLevel_Stage1::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE2:
			pNewLevel = CLevel_Stage2::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE3:
			pNewLevel = CLevel_Stage3::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE4:
			pNewLevel = CLevel_Stage4::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE5:
			pNewLevel = CLevel_Stage5::Create(m_pGraphic_Device);
			break;
		case LEVEL::LEVEL_STAGE6:
			pNewLevel = CLevel_Stage6::Create(m_pGraphic_Device);
			break;

		}

		if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(m_eNextLevelID), pNewLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Show_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_GameObjects()
{
	return S_OK;
}

HRESULT CLevel_Loading::Ready_LoadingThread()
{
	m_pLoader= CLoader::Create(m_pGraphic_Device, m_eNextLevelID);

	if (m_pLoader == nullptr)
		return E_FAIL;

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphicDev);
	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pLoader);

}
