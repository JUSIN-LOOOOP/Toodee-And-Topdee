#include "Loader.h"
#include "GameInstance.h"

#pragma region Block
#include "Block_Break.h"
#include "Block_Lock.h"
#include "Block_Wood.h"
#include "Block_Fall.h"
#include "Block_Spark.h"
#include "Block_Metal.h"
#pragma endregion

#pragma region Object
#include "Player_Thirdee.h"
#include "Hole.h"
#include "Key.h"
//#include "Cloud.h"
#include "Storm.h"
#include "Rain.h"
#include "RainSplash.h"
#include "Lightning.h"
#include "Sink.h"
#include "Water.h"
#include "Cannon.h"
#include "BackCloud.h"
#include "Fire_Projectile.h"
#include "SpikeHole.h"
#pragma endregion

#pragma region Boss
#include "StageBoss.h"
#include "StageBoss_Body.h"
#include "StageBoss_Hand.h"
#pragma endregion

#pragma region Monster
#include "Pig.h"
#include "Bat.h"
#pragma endregion

#pragma region ETC
#include "Camera.h"
#include "FPSCamera.h"
#include "TextureUI.h"
#include "BasicTile.h"
#include "TileOutline.h"
#include "FireBall.h"
#include "Semicolon.h"
#include "Finger.h"
#include "Toodoo.h"
#include "Test_Cube.h"
#include "Test_Cube2.h"
#include "Collider_Cube.h"
#include "VIBuffer_Terrain.h"
#pragma endregion

#pragma region MAINMENU
#include "MainMenu_BackGround.h"
#include "MainMenu_Spark.h"
#include "MainMenu_CenterLine.h"
#include "MainMenu_Title.h"
#include "MainMenu_Buttons.h"
#include "MainMenu_Portals.h"
#include "MainMenu_StageTitle.h"

#pragma endregion


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
	case LEVEL::LEVEL_STAGE1:
		hr = Loading_For_Stage1();
		break;
	case LEVEL::LEVEL_STAGE2:
		hr = Loading_For_Stage2();
		break;
	case LEVEL::LEVEL_STAGE3:
		hr = Loading_For_Stage3();
		break;
	case LEVEL::LEVEL_STAGE4:
		hr = Loading_For_Stage4();
		break;
	case LEVEL::LEVEL_STAGE5:
		hr = Loading_For_Stage5();
		break;
	case LEVEL::LEVEL_STAGE6:
		hr = Loading_For_Stage6();
		break;
	case LEVEL::LEVEL_STAGEBOSS:
		hr = Loading_For_StageBoss();
		break;
	case LEVEL::LEVEL_FINALBOSS1:
		hr = Loading_For_FinalBoss01();
		break;
	case LEVEL::LEVEL_FINALBOSS2:
		hr = Loading_For_FinalBoss02();
		break;
	case LEVEL::LEVEL_FINALBOSS3:
		hr = Loading_For_FinalBoss03();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	/* BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/BackGround/MainMenu_BackGround.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_BackGround"),
		CMainMenu_BackGround::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* CenterLine */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_CenterLine"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/CenterLine/CenterLine%d.png"), 6))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_CenterLine"),
		CMainMenu_CenterLine::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Title */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Title"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/Title/Title%d.png"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_Title"),
		CMainMenu_Title::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Buttons */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Buttons"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/Buttons/MainMenu_Button%d.png"), 2))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Arrow"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/MenuArrow/MainMenu_Arrow%d.png"), 2))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_Buttons"),
		CMainMenu_Buttons::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Portal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_StagePortal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/Stage_Select/LevelPortal/levelPortalSpr_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_StageImage"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/Stage_Select/StageImage/StageImage%d.png"), 8))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_Portal"),
		CMainMenu_Portals::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Stage selected Text*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_LevelText"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/Font/Level/LevelText%d.png"), 3))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_Component_Texture_MainMenu_Number"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/MainMenu/Font/Number/%d.png"), 10))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_LOGO), TEXT("Prototype_GameObject_MainMenu_StageTitle"),
		CMainMenu_StageTitle::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("Now Loading Texture..."));

#pragma region TEXTURE BLOCK
	/* Prototype_Component_Texture_TestCube */
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_TestCube"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Wall/test1.dds"), 1))))
		return E_FAIL;*/

#pragma endregion

#pragma region TEXTURE_STAGEBOSS
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_StageBoss_Body"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/StageBoss/StageBoss.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_StageBoss_Parts"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/StageBoss/StageBossParts%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_FireBall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/StageBoss/FireballSpr_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_StageBoss_Hand"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/StageBoss/StageBoss_hand.dds"), 1))))
		return E_FAIL;

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("Now Loading Model..."));

	lstrcpy(m_szLoadingText, TEXT("Now Loading Shader..."));

	lstrcpy(m_szLoadingText, TEXT("Now Loading GameObejct..."));
	/* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestCube*/
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube"),
		CTest_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_TestCube2*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_TestCube2"),
		CTest_Cube2::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_ColliderCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_ColliderCube"),
		CCollider_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region BLOCK
  
	/* Prototype_GameObject_Block*/
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
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Block_Wood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

  
#pragma endregion
  
	/* Prototype_GameObject_TestTile*/
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Tile"),
		CBasicTile::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Monster_Pig */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Pig"),
		CPig::Create(m_pGraphic_Device))))
		return E_FAIL;
  /* Prototype_GameObject_BackCloud */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_BackCloud"),
		CBackCloud::Create(m_pGraphic_Device))))
		return E_FAIL;
  

#pragma region GameObject_StageBoss

	///* Prototype_GameObject_StageBoss*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_StageBoss"),
		CStageBoss::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_StageBoss_Body*/
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_StageBoss_Body"),
		CStageBoss_Body::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_FireBall"),
		CFireBall::Create(m_pGraphic_Device))))
		return E_FAIL;

	///* Prototype_GameObject_StageBoss_Hand*/
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_StageBoss_Hand"),
		CStageBoss_Hand::Create(m_pGraphic_Device))))
		return E_FAIL;*/


#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("The Gameplay_Level has finished loading."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MapEdit_Level()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Tile"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Tile/Tile%d.png"), 112))))
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

HRESULT CLoader::Loading_For_Stage1()
{
#pragma region TEXTURE

#pragma endregion

#pragma region GAMEOBJECT

	/* Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallWood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallBreak"),
		CBlock_Break::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_BackCloud */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE1), TEXT("Prototype_GameObject_BackCloud"),
		CBackCloud::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion



	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage2()
{
	/* Prototype_GameObject_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallBreak"),
		CBlock_Break::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Fall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_WallFall"),
		CBlock_Fall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

#pragma region TEXTURE Back

	/* Prototype_Component_Texture_BackCloud */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE2), TEXT("Prototype_GameObject_BackCloud"),
		CBackCloud::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage3()
{
	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region Monster

	/* Prototype_GameObject_Pig */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE3), TEXT("Prototype_GameObject_Pig"),
		CPig::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage4()
{
#pragma region TEXTURE_MONSTER(Bat)

	/* Prototype_Component_Texture_Bat_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Body"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Bat/BatBodySpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Bat_Ears */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Ears"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Bat/BatEarsSpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Bat_Eyes */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Eyes"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Bat/BatEyesSpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Bat_Legs */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Legs"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Bat/BatLegsSpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Bat_Nose */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Nose"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Bat/BatNoseSpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Bat_Tail */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Wing"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Bat/BatWingSpr_%d.png"), 6))))
		return E_FAIL;

#pragma endregion

	/* Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_WallWood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_WallBreak"),
		CBlock_Break::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region Object_Monster

	/* Prototype_GameObject_Pig */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Pig"),
		CPig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Bat */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_GameObject_Bat"),
		CBat::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage5()
{
	/* Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_WallWood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Metal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_Metal"),
		CBlock_Metal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_Sink"),
		CSink::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 20, 20))))
		return E_FAIL;

	/* Prototype_Component_Texture_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_Texture_Water"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Water/Water_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Sink */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_Texture_Sink"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Water/Sink_0.png"), 1))))
		return E_FAIL;
	
	/* Prototype_Component_Shader_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_Shader_Water"),
		CShader_Water::Create(m_pGraphic_Device, TEXT("../Resources/Shader/Water.txt")))))
		return E_FAIL;



	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage6()
{
	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Fall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_WallFall"),
		CBlock_Fall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Spark"),
		CBlock_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;




#pragma region TEXTURE_CLOUD

	/* Prototype_Component_Texture_Rain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_Component_Texture_Rain"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Rain/Rain.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_RainSplash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_Component_Texture_RainSplash"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/RainSplash/RainSplash%d.png"), 6))))
		return E_FAIL;

	/* Prototype_Component_Texture_Lightning */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_Component_Texture_Lightning"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Lightning/Lightning%d.png"), 8))))
		return E_FAIL;

#pragma endregion

#pragma region GameObject_Cloud

	/* Prototype_GameObject_Storm */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Storm"),
		CStorm::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* Prototype_GameObject_Rain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Rain"),
		CRain::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* Prototype_GameObject_RainSplash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_RainSplash"),
		CRainSplash::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* Prototype_GameObject_Lightning */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGE6), TEXT("Prototype_GameObject_Lightning"),
		CLightning::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_StageBoss()
{
	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Body"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/StageBoss/StageBoss.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Parts"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/StageBoss/StageBossParts%d.png"), 8))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_FireBall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/StageBoss/FireballSpr_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Hand"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/StageBoss/StageBoss_hand.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Chain"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/StageBoss/StageBossChain%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_StageBoss"),
		CStageBoss::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_FireBall"),
		CFireBall::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_FinalBoss01()
{
	/* Prototype_GameObject_SpikeHole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_SpikeHole"),
		CSpikeHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_WallWood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Fall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_WallFall"),
		CBlock_Fall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_Spark"),
		CBlock_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_FPSCamera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_FPSCam"),
		CFPSCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Semicolon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_Semiclon"),
		CSemicolon::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Finger */
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_Finger"),
		CFinger::Create(m_pGraphic_Device))))
 		return E_FAIL;

	/* Prototype_GameObject_Toodoo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1), TEXT("Prototype_GameObject_Toodoo"),
		CToodoo::Create(m_pGraphic_Device))))
		return E_FAIL;

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_FinalBoss02()
{
	/* Prototype_GameObject_SpikeHole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_SpikeHole"),
		CSpikeHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_WallBreak"),
		CBlock_Break::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Spark"),
		CBlock_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Metal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Metal"),
		CBlock_Metal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_FPSCamera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_FPSCam"),
		CFPSCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Semicolon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Semiclon"),
		CSemicolon::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Finger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Finger"),
		CFinger::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Toodoo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS2), TEXT("Prototype_GameObject_Toodoo"),
		CToodoo::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_FinalBoss03()
{	
	/* Prototype_GameObject_SpikeHole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_SpikeHole"),
		CSpikeHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_WallBreak"),
		CBlock_Break::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_Hole"),
		CHole::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_Key"),
		CKey::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_WallLock"),
		CBlock_Lock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_Spark"),
		CBlock_Spark::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_WallWood"),
		CBlock_Wood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_FPSCamera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_FPSCam"),
		CFPSCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Semicolon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_Semiclon"),
		CSemicolon::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Finger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_Finger"),
		CFinger::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Toodoo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_GameObject_Toodoo"),
		CToodoo::Create(m_pGraphic_Device))))
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
