#include "Pool_Manager.h"
#include "Pool.h"
#include "PoolableObject.h"
#include "GameInstance.h"
CPool_Manager::CPool_Manager()
	:m_pGameInstance { CGameInstance::GetInstance() }
{
}


HRESULT CPool_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	m_pPools = new map<const _wstring, class CPool*>[iNumLevels];

	return S_OK;
}

void CPool_Manager::Push(_uint iNumLevels,const _wstring& strPoolTag, CPoolableObject* pGameObject)
{
	CPool* pPool = Find_Pools(iNumLevels, strPoolTag);
	if (pPool == nullptr)
	{
		pPool = CPool::Create();
  		m_pPools[iNumLevels].emplace(strPoolTag, pPool);
	}

	if (pGameObject != nullptr) {
		pGameObject->Set_Active(false);
		pPool->Push(pGameObject);
	}

}

CPoolableObject* CPool_Manager::Pop(_uint iNumLevels, const _wstring& strPoolTag)
{
	CPool* pPool = Find_Pools(iNumLevels, strPoolTag);
	if (pPool != nullptr) {
		CPoolableObject* object = pPool->Pop();
		if(object != nullptr)
			object->Set_Active(true);
		return object;
	}
	else
	{
		if (strPoolTag == TEXT("Layer_Projectile_Fire"))
		{
			m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strPoolTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Projectile_Fire"));
		}
		if (strPoolTag == TEXT("Layer_Projectile_Laser")) 
		{
			m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), strPoolTag,
				ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_GameObject_Projectile_Laser"));
		}

		//return Pop(iNumLevels, strPoolTag);
		MSG_BOX(TEXT("Failed to Pop : CPool_Manager"));

	}

}

void CPool_Manager::Clear(_uint iNumLevels)
{
	if (iNumLevels >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPools[iNumLevels]) 
		Safe_Release(Pair.second);
	
	m_pPools[iNumLevels].clear();
}

CPool* CPool_Manager::Find_Pools(_uint iNumLevels, const _wstring strPoolTag)
{
	auto iter = m_pPools[iNumLevels].find(strPoolTag);
	if (iter == m_pPools[iNumLevels].end())
		return nullptr;

	return iter->second;
}

CPool_Manager* CPool_Manager::Create(_uint iNumLevels)
{
	CPool_Manager* pInstance = new CPool_Manager;
	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CPool_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPool_Manager::Free()
{
	__super::Free();
	for (_uint i = 0; i < m_iNumLevels; ++i) {
		for (auto& iter : m_pPools[i]) {
			Safe_Release(iter.second);
		}
		m_pPools[i].clear();
	}
	Safe_Release(m_pGameInstance);
	Safe_Delete_Array(m_pPools);
}
