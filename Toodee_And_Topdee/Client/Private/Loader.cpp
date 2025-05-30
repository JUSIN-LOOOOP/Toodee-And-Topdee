#include "Loader.h"
#include "GameInstance.h"

#include "Camera.h"
#include "MultiViewCamera.h"
#include "Player_Toodee.h"
#include "Player_Topdee.h"
#include "BasicTile.h"
#include "Potal.h"
#include "Block_Wall.h"
#include "Block_Break.h"
#include "Block_Lock.h"
#include "Block_Wood.h"
#include "Block_Fall.h"
#include "Block_Spark.h"
#include "Block_Metal.h"
#include "Hole.h"
#include "TextureUI.h"

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
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("���� �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("���̴��� �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("���ӿ�����Ʈ������ �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("Logo_Level �ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�."));

#pragma region TEXTURE BLOCK
	/* Prototype_Component_Texture_TestCube */
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_TestCube"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Wall/test1.dds"), 1))))
		return E_FAIL;*/

	/* Prototype_Component_Texture_Wall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Wall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Wall/Wall%d.dds"), 93))))
		return E_FAIL;


	/* Prototype_Component_Texture_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Wood"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/WoodBox.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Break"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Breakable.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Lock"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Lock.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Fall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Fall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Fall.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Spark"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Spark%d.dds"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Metal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Metal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Metal.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Block/Hole.png"), 1))))
		return E_FAIL;

#pragma endregion

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

	lstrcpy(m_szLoadingText, TEXT("���� �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("���̴��� �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("���ӿ�����Ʈ������ �ε����Դϴ�."));
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
#pragma region BLOCK
	/* Prototype_GameObject_Block*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallBlock"),
		CBlock_Wall::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallWood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallBreak"),
		CBlock_Break::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_WallFall"),
		CBlock_Fall::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Spark"),
		CBlock_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Metal"),
		CBlock_Metal::Create(m_pGraphic_Device))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

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

	lstrcpy(m_szLoadingText, TEXT("GamePlay_Level �ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapEdit_Level()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Tile"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Tile/Tile%d.png"), 101))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_MultiViewCamera"),
		CMultiViewCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_TextureUI"),
		CTextureUI::Create(m_pGraphic_Device))))
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
