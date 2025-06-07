#include "Object_Manager.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "PoolableObject.h"

CObject_Manager::CObject_Manager()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	m_pLayers = new map<const _wstring, class CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	// Prototype_Manager에서 사본 게임 오브젝트를 생성하여 가지고 온다.
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	//가지고 온 사본 게임오브젝트가 Layer에 있는지 검사하여 Layer에 보관
	//만약 처음 가지고 온거라면 m_pLayers에도 보관
	CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		pLayer->Add_GameObject(pGameObject);
		m_pLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	//Map Edit 레벨일 때, MapManager가 Tile객체를 관리할 수 있도록 포인터를 넘겨준다.
	if (strLayerTag == L"Layer_Tile")
		m_pGameInstance->Add_Tile(pGameObject);

	/* Object Pooling */
	if (strLayerTag == L"Layer_Projectile_Fire")
		m_pGameInstance->First_Push(strLayerTag,dynamic_cast<CPoolableObject*>(pGameObject));

	if (strLayerTag == L"Layer_Projectile_Laser")
		m_pGameInstance->First_Push(strLayerTag, dynamic_cast<CPoolableObject*>(pGameObject));

	if (strLayerTag == L"Layer_Rain")
		m_pGameInstance->First_Push(strLayerTag, dynamic_cast<CPoolableObject*>(pGameObject));

	if (strLayerTag == L"Layer_RainSplash")
		m_pGameInstance->First_Push(strLayerTag, dynamic_cast<CPoolableObject*>(pGameObject));

	if (strLayerTag == L"Layer_Lightning")
		m_pGameInstance->First_Push(strLayerTag, dynamic_cast<CPoolableObject*>(pGameObject));

	return S_OK;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->Priority_Update(fTimeDelta);
		}
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->Update(fTimeDelta);
		}
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (nullptr != Pair.second)
				Pair.second->Late_Update(fTimeDelta);
		}
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for(auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

CComponent* CObject_Manager::Get_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);
}

CGameObject* CObject_Manager::Get_BackGameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (pLayer != nullptr)
		return pLayer->Get_BackGameObject();

	return nullptr;
}

CLayer* CObject_Manager::Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	auto it = m_pLayers[iLayerLevelIndex].find(strLayerTag);

	if (it != m_pLayers[iLayerLevelIndex].end())
		return it->second;

	return nullptr;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager* pInstance = new CObject_Manager;
	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CObject_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Release(m_pGameInstance);

	Safe_Delete_Array(m_pLayers);
}
