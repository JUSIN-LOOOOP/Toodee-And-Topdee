#include "Loader.h"
#include "GameInstance.h"

#include "Camera.h"
#include "MultiViewCamera.h"
#include "Player_Toodee.h"
#include "Player_Topdee.h"
#include "BasicTile.h"
#include "Potal.h"
#include "TileOutline.h"

#include "Test_Cube.h"
#include "Test_Cube2.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device{ pGraphic_Device },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initailize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;
	case LEVEL::LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	case LEVEL::LEVEL_MAPEDIT:
		hr = Loading_For_MapEdit_Level();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트원형를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("Logo_Level 로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	/* Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_TestCube"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Test/Test0.dds"), 1))))
		return E_FAIL;

	if(FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_TileOutline"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Test/TileOutline%d.png"), 4))))
		return E_FAIL;

#pragma region TEXTURE TOODEE
	/* Prototype_Component_Texture_Toodee_Idle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Idle%d.png"), 21))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Move */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Move%d.png"), 12))))
		return E_FAIL;	
	/* Prototype_Component_Texture_Toodee_Action */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Action"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Action%d.png"), 5))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Stop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Stop"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Outline%d.png"), 46))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Clear */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Clear"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodeeSwirl%d.png"), 17))))
		return E_FAIL;
#pragma endregion

#pragma region TEXTURE TOPDEE
	/* Prototype_Component_Texture_Topdee_Idle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Idle%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Move */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Move%d.png"), 40))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Action */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Action"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Idle%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Stop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Stop"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Outline%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Clear */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Topdee_Clear"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Swir%d.png"), 17))))
		return E_FAIL;

#pragma endregion

	

	/* Prototype_Component_Texture_Potal */
	if(FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Potal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Potal/portalSpr_%d.png"),11))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("쉐이더를 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("게임오브젝트원형를 로딩중입니다."));
	/* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_MultiViewCamera"),
		CMultiViewCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube"),
		CTest_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestCube2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube2"),
		CTest_Cube2::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestTile*/
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Tile"),
		CBasicTile::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Player_Toodee */
	if(FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Player_Toodee"),
		CPlayer_Toodee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Player_Topdee */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Player_Topdee"),
		CPlayer_Topdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Potal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TileOutline */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TileOutline"),
		CTileOutline::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("GamePlay_Level 로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapEdit_Level()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Tile"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Test/Tile.ver/TileSpr_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_MultiViewCamera"),
		CMultiViewCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_TestCube"),
		CTest_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestTile*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Tile"),
		CBasicTile::Create(m_pGraphic_Device))))
		return E_FAIL;


	m_isFinished = true;
	return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphicDev);

	if (FAILED(pInstance->Initailize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
