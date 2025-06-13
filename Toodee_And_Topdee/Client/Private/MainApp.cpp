
#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "MultiViewCamera.h"
#include "LoadingScreen.h"

#include "Backdrop.h"
#include "BackWall.h"
#include "BackTile.h"
#include "Block_Wall.h"
#include "Block_Disappear.h"
#include "RedButton.h"

#include "Part_Body.h"
#include "Part_Eyes.h"
#include "Part_Nose.h"
#include "Part_Tail.h"
#include "Part_Legs.h"
#include "Part_Ears.h"
#include "Part_WIng.h"

#include "Player_Toodee.h"
#include "Player_Topdee.h"
#include "Player_Thirdee.h"

#include "Potal.h"
#include "TileOutline.h"
#include "Spikes.h"
#include "Cloud.h"
#include "Fire_Projectile.h"
#include "Cannon.h"
#include "FinalBoss.h"

#include "ColliderMap_Object.h"

Client::CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWinMode = WINMODE::MODE_WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::LEVEL_END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_ForStatic()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_ForStatic_Background()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_ForStatic_Gimmick()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_ForStatic_Player()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_ForStatic_Monster()))
		return E_FAIL;

	if (FAILED(Ready_Layer_LoadingScreen(TEXT("Layer_LoadingScreen"))))
		return E_FAIL;



	m_pGameInstance->Subscribe<LEVELCHANGE_EVENT>(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, [this](const LEVELCHANGE_EVENT& Event) {
		this->Ready_Open_Level(Event);
		});

	m_bChangeLevel = false;

	if (FAILED(Start_Level(LEVEL::LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Post_Update()
{
	if (m_bChangeLevel)
	{
		m_pGameInstance->Clear_Resources(m_iCurrentLevel);
		m_pGameInstance->Open_Level(ENUM_CLASS(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, static_cast<LEVEL>(m_iChangeLevel)));
		m_bChangeLevel = false;
	}

}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pGameInstance->Draw();

	m_pGameInstance->Render_End();

	return S_OK;
}

void CMainApp::Ready_Open_Level(const LEVELCHANGE_EVENT& Event)
{
	m_iChangeLevel = Event.iChangeLevel;
	m_iCurrentLevel = Event.iCurrentLevel;
	m_bChangeLevel = true;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	/* 텍스쳐 1 초과시 0으로 다시 시작*/
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_ForStatic()
{
	/* Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Sphere"),
		CVIBuffer_Sphere::Create(m_pGraphic_Device, 20, 20))))
		return E_FAIL;

	/* Prototype_Component_VIBuffer_Capsule */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Capsule"),
		CVIBuffer_Capsule::Create(m_pGraphic_Device, 20, 20))))
		return E_FAIL;

	/* Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Collider_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Rect"),
		CCollider::Create(m_pGraphic_Device,COLLIDER_SHAPE::RECT))))
		return E_FAIL;

	/* Prototype_Component_Collider_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		CCollider::Create(m_pGraphic_Device, COLLIDER_SHAPE::CUBE))))
		return E_FAIL;
  
	/* Prototype_GameObject_Camera*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_MultiViewCamera"),
		CMultiViewCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_FinalBoss */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_FinalBoss"),
		CFinalBoss::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Texture_FinalBoss_Attack*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_FinalBoss_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/FinalBoss/Attack/%d.png"), 11))))
		return E_FAIL;

	/* Prototype_Component_Texture_Effect_FinalBoss_Attack*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_FinalBoss_Attack"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Effect/Eff_FinalBoss_Attack/%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Effect_FinalBoss_Attack*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_FinalBoss_Attack_Projectile"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Effect/Eff_FinalBoss_Attack/Projectile.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Semicolon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Semiclon"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/FinalBoss/semicolon.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Finger*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Finger"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/FinalBoss/Finger.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_LoadingScreen*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_LoadingScreen"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/LoadingScreen/%d.png"), 13))))
		return E_FAIL;

	/* Prototype_GameObject_LoadingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_LoadingScreen"),
		CLoadingScreen::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}
HRESULT CMainApp::Ready_Prototype_ForStatic_Background()
{
#pragma region TEXTURE_MAP_OBJECT

	/* Prototype_Texture_Block_Wall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Wall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Wall/Wall%d.dds"), 99))))
		return E_FAIL;

	/* Prototype_Component_Texture_Break */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Break"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Breakable.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Lock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Lock"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Lock.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Fall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Fall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Fall.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Spark"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Spark%d.dds"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Metal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Metal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Metal.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Hole */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Hole"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Block/Hole.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Key */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Key"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Key/keySpr_%d.png"), 12))))
		return E_FAIL;

	/* Prototype_Component_Texture_BackWall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BackWall"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Map/Backtile%d.png"), 3))))
		return E_FAIL;
	/* Prototype_Component_Texture_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Wood"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/WoodBox.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Backdrop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Backdrop"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Map/Backdrop%d.png"), 4))))
		return E_FAIL;

	/* Prototype_Component_Texture_BackTile */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BackTile"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Map/Tile.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_BackCloud */

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BackCloud"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Map/BackCloud.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodoo"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Map/Boss.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Spikes */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Spikes"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Spikes/Spikes%d.png"), 6))))
		return E_FAIL;

	/* Prototype_Component_Texture_Block_Disappear_Side */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Disappear_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Disappear_Side.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Block_Disappear_Center */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Disappear_Center"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::CUBE, TEXT("../Resources/Textures/Block/Disappear_Center.dds"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_RedButton_ButtonFace */
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_RedButton_ButtonFace"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/RedButton/ButtonFace%d.png"), 4))))
		return E_FAIL;

	/* Prototype_Component_Texture_RedButton_ButtonPush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_RedButton_ButtonPush"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/RedButton/ButtonPush%d.png"), 2))))
		return E_FAIL;

#pragma endregion

#pragma region GAMEOBJECT_MAP_OBJECT

	/* Prototype_GameObject_Collider_Map */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Collider_Map"),
		CColliderMap_Object::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_BackWall */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackWall"),
		CBackWall::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* Prototype_GameObject_BackDrop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackDrop"),
		CBackdrop::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_BackTile */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_BackTile"),
		CBackTile::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_WallBlock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_WallBlock"),
		CBlock_Wall::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Spikes */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Spikes"),
		CSpikes::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Block_Disappear */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Block_Disappear"),
		CBlock_Disappear::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* Prototype_GameObject_RedButton */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_RedButton"),
		CRedButton::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_ForStatic_Gimmick()
{	
#pragma region TEXTURE_Clouds
	/* Prototype_Component_Texture_Cloud */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cloud"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Clouds/Cloud%d.png"), 6))))
		return E_FAIL;

	/* Prototype_Component_Texture_DarkCloud */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_DarkCloud"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Clouds/DarkCloud%d.png"), 6))))
		return E_FAIL;
#pragma endregion

#pragma region GAMEOBJECT_CLOUD

	/* Prototype_GameObject_Cloud*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cloud"),
		CCloud::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

#pragma region TEXTURE_CANNON
	/* Prototype_Component_Texture_Cannon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cannon"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Cannon/Cannon%d.png"), 4))))
		return E_FAIL;

	/* Prototype_Component_Texture_Projectile_Fire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Projectile_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Projectile/Fire/Projectile_Fire%d.png"), 40))))
		return E_FAIL;

	/* Prototype_Component_Texture_Effect_Cannon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_Cannon"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Effect/Eff_Cannon/%d.png"), 3))))
		return E_FAIL;

	/* Prototype_Component_Texture_Effect_Projectile */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_Projectile"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Effect/Eff_Projectile/%d.png"), 9))))
		return E_FAIL;

#pragma endregion

#pragma region GameObject_Cannon

	/* Prototype_GameObject_Cannon*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Cannon"),
		CCannon::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Projectile_Fire*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Projectile_Fire"),
		CFire_Projectile::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_ForStatic_Player()
{
#pragma region TEXTURE TOODEE
	/* Prototype_Component_Texture_Toodee_Idle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Idle%d.png"), 21))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Move */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Move%d.png"), 12))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Action */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Action"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Action%d.png"), 5))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Swim */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Swim"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Swim%d.png"), 8))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Stop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Stop"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Outline%d.png"), 46))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Clear */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Clear"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodeeSwirl%d.png"), 17))))
		return E_FAIL;
	/* Prototype_Component_Texture_Toodee_Dead */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Dead"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Toodee/toodee_Dead%d.png"), 5))))
		return E_FAIL;
#pragma endregion

#pragma region TEXTURE TOPDEE
	/* Prototype_Component_Texture_Topdee_Idle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Idle"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Idle%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Move */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Move"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Move%d.png"), 40))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Action */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Action"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Idle%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Stop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Stop"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Outline%d.png"), 5))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Clear */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Clear"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Swir%d.png"), 17))))
		return E_FAIL;

	/* Prototype_Component_Texture_Topdee_Dead*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Topdee_Dead"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Topdee/Topdee_Dead%d.png"), 5))))
		return E_FAIL;
#pragma endregion

#pragma region TEXTURE THIRDEE
	/* Prototype_Component_Texture_Thirdee_Head*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Thirdee_Head"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Thridee/ThirdeeHead.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Thirdee_Body*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Thirdee_Body"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Thridee/ThirdeeBody.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Thirdee_Arm*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Thirdee_Arm"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Thridee/ThirdeeArm.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Thirdee_Leg*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Thirdee_Leg"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Player/Thridee/ThirdeeLeg.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region GAMEOBJECT_PLAYERS

	/* Prototype_GameObject_Player_Toodee */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Toodee"),
		CPlayer_Toodee::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_GameObject_Player_Topdee */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Topdee"),
		CPlayer_Topdee::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Player_Thirdee"),
		CPlayer_Thirdee::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma region PLAYERS_ETC...
	/* Prototype_Component_Texture_Potal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Potal"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Potal/portalSpr_%d.png"), 11))))
		return E_FAIL;

	/* Prototype_GameObject_Potal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Potal"),
		CPotal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototypee_Component_Texture_TileOutline */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_TileOutline"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Test/TileOutline%d.png"), 4))))
		return E_FAIL;

	/* Prototype_GameObject_TileOutline */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_TileOutline"),
		CTileOutline::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}


HRESULT CMainApp::Ready_Prototype_ForStatic_Monster()
{
#pragma region PigTexture

	/* Prototype_Component_Texture_Pig_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Body"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Pig/PigBodySpr_%d.png"), 10))))
		return E_FAIL;

	/* Prototype_Component_Texture_Pig_Ears */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Ears"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Pig/PigEarsSpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Pig_Eyes */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Eyes"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Pig/PigEyesSpr_%d.png"), 2))))
		return E_FAIL;

	/* Prototype_Component_Texture_Pig_Legs */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Legs"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Pig/PigLegsSpr_%d.png"), 9))))
		return E_FAIL;

	/* Prototype_Component_Texture_Pig_Nose */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Nose"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Pig/PigNoseSpr_0.png"), 1))))
		return E_FAIL;

	/* Prototype_Component_Texture_Pig_Tail */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Tail"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::RECT, TEXT("../Resources/Textures/Monster/Pig/PigTailSpr_0.png"), 1))))
		return E_FAIL;

#pragma endregion

#pragma region Monster Parts

	/* Prototype_Component_Part_Eyes */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		CPart_Eyes::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Part_Nose */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		CPart_Nose::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Part_Tail */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		CPart_Tail::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Part_Legs */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		CPart_Legs::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Part_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		CPart_Body::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Part_Ears */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		CPart_Ears::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Part_Wing */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Wing"),
		CPart_Wing::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

	return S_OK;
}



HRESULT CMainApp::Start_Level(LEVEL eStartLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(static_cast<_uint>(LEVEL::LEVEL_LOADING), CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Layer_LoadingScreen(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(	ENUM_CLASS(LEVEL::LEVEL_STATIC), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_LoadingScreen"))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	__super::Free();
	//Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Clear_Resources(ENUM_CLASS(LEVEL::LEVEL_STATIC));

	m_pGameInstance->Release_Engine();

    Safe_Release(m_pGameInstance);

}
