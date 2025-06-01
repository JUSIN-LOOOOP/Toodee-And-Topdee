#include "GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Renderer.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Key_Manager.h"
#include "Timer_Manager.h"
#include "Map_Manager.h"
#include "Collision_Manager.h"
#include "Observer_Manager.h"
#include "Sound_Manager.h"
#include "Pool_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

#pragma region ENGINE

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWinMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pKey_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;
	m_eCurrentDimension = DIMENSION::TOODEE;

	m_pMap_Manager = CMap_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pCollision_Manager = CCollision_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pCollision_Manager)
		return E_FAIL;

 	m_pObserver_Manager = CObserver_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pSound_Manager = CSound_Manager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	m_pPool_Manager = CPool_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPool_Manager)
		return E_FAIL;

    return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pKey_Manager->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);


	m_pLevel_Manager->Update(fTimeDelta);

	m_pMap_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Clear_Resources(_uint iClearLevelID)
{
	m_pCollision_Manager->Clear(iClearLevelID);

	m_pPrototype_Manager->Clear(iClearLevelID);

	m_pObject_Manager->Clear(iClearLevelID);

	m_pObserver_Manager->Clear(iClearLevelID);

	m_pPool_Manager->Clear(iClearLevelID);

    return S_OK;
}


void CGameInstance::Render_Begin(D3DXCOLOR Color)
{
	if (m_pGraphic_Device != nullptr) 
		m_pGraphic_Device->Render_Begin(Color);
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pLevel_Manager || nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw();

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr != m_pGraphic_Device)
		m_pGraphic_Device->Render_End(hWnd);
}

_float CGameInstance::Rand_Normal()
{
	return static_cast<_float>(rand()) / RAND_MAX;
}

_float CGameInstance::Rand(_float fMin, _float fMax)
{
	return fMin + Rand_Normal() * (fMax - fMin);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel* pNewLevel)
{
	if (m_pLevel_Manager == nullptr)
		return E_FAIL;

    return m_pLevel_Manager->Open_Level(iLevelID, pNewLevel);
}

const _uint CGameInstance::Get_CurrentLevelID()
{
	return m_pLevel_Manager->Get_CurrentLevelID();
}


#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	return m_pPrototype_Manager->Add_Prototype(iPrototpyeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(ePrototype, iPrototpyeLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region OBJECT_MANAGER



CComponent* CGameInstance::Find_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLayerLevelIndex, strLayerTag, strComponentTag, iIndex);

}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, iPrototypeLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);

}

#pragma endregion

#pragma region KEY_MANAGER

_bool CGameInstance::Key_Pressing(int _iKey)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Pressing(_iKey);
}

_bool CGameInstance::Key_Up(int _iKey)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Up(_iKey);
}

_bool CGameInstance::Key_Down(int _iKey)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Down(_iKey);
}


#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion

#pragma region COLLISION_MANAGER

HRESULT CGameInstance::Add_Collider(_uint iLevelIndex, COLLIDER_SHAPE etype, CCollider** pCollider)
{
	if (nullptr == m_pCollision_Manager)
		return E_FAIL;

	return m_pCollision_Manager->Add_Collider(iLevelIndex, etype, pCollider);
}

void CGameInstance::Delete_Collider(_uint iLevelIndex, CCollider** pCollider)
{
	if (nullptr == m_pCollision_Manager)
		return ;

	return m_pCollision_Manager->Delete_Collider(iLevelIndex, pCollider);
}

void CGameInstance::Check_Collision(CCollider* pCollider)
{
	if (nullptr == m_pCollision_Manager)
		return;

	return m_pCollision_Manager->Check_Collision(Get_CurrentLevelID() - 1, pCollider);
}

#pragma endregion

#pragma region OBSERVER_MANAGER

HRESULT CGameInstance::Add_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, CObserver* pObserver)
{
	return m_pObserver_Manager->Add_Observer(iObserverLevelndex, strObserverTag, pObserver);
}

HRESULT CGameInstance::Subscribe_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, CSubjectObject* pSubject)
{
	return m_pObserver_Manager->Subscribe_Observer(iObserverLevelndex, strObserverTag, pSubject);
}

#pragma endregion

#pragma region SOUND_MANAGER

void CGameInstance::PlayAudio(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;
	m_pSound_Manager->PlayAudio(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return;
	m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
		return;
	m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;
	m_pSound_Manager->SetChannelVolume(eID, fVolume);

}

#pragma endregion

#pragma region POOL_MANAGER

void CGameInstance::First_Push(const _wstring& strPoolTag, class CPoolableObject* pGameObject)
{
	if (nullptr == m_pPool_Manager)
		return;

	m_pPool_Manager->Push(Get_CurrentLevelID() + 1, strPoolTag, pGameObject);
}

void CGameInstance::Push(const _wstring& strPoolTag, CPoolableObject* pGameObject)
{
	if (nullptr == m_pPool_Manager)
		return;

	m_pPool_Manager->Push(Get_CurrentLevelID() - 1,strPoolTag, pGameObject);
}

CPoolableObject* CGameInstance::Pop(const _wstring& strPoolTag)
{
	if (nullptr == m_pPool_Manager)
		return nullptr;

	return m_pPool_Manager->Pop(Get_CurrentLevelID() - 1, strPoolTag);
}

#pragma endregion


#pragma region MAP_MANAGER
HRESULT CGameInstance::Load_File(const _wstring& filename)
{
	m_pMap_Manager->Load_File(filename);
	return S_OK;
}

BLOCK_INFO CGameInstance::Get_CurrentType()
{
	return m_pMap_Manager->Get_CurrentType();
}

HRESULT CGameInstance::Add_Tile(CGameObject* tile)
{
	m_pMap_Manager->Add_Tiles(tile);
	Safe_AddRef(tile);

	return S_OK;
}
#pragma endregion
HRESULT CGameInstance::Get_Tile_Data(_int idx, BLOCK_INFO& block_data)
{
	return m_pMap_Manager->Get_Tile_Data(idx, block_data);
}

HRESULT CGameInstance::Load_Initial_Data(vector<_uint>* blockData)
{
	return m_pMap_Manager->Load_Initial_Data(blockData);
}

void CGameInstance::Release_Engine()
{
	Release();

	/*오브젝트가 사용하는 의존성들 먼저 유지하고 오브젝트 먼저 해제할게요*/
	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pMap_Manager);
	Safe_Release(m_pObserver_Manager);
	Safe_Release(m_pPool_Manager);
}

void CGameInstance::Free()
{
	__super::Free();
}
