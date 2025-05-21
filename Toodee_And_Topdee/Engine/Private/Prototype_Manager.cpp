#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Component.h"

CPrototype_Manager::CPrototype_Manager()
{
}

HRESULT CPrototype_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	m_pPrototypes = new PROTOTYPES[iNumLevels];

	return S_OK;
}

HRESULT CPrototype_Manager::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	if (nullptr == m_pPrototypes ||
		m_iNumLevels <= iPrototypeLevelIndex ||
		nullptr != Find_Prototype(iPrototypeLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iPrototypeLevelIndex].emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

CBase* CPrototype_Manager::Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CBase* pBase = Find_Prototype(iPrototypeLevelIndex, strPrototypeTag);
	if (pBase == nullptr)
		return nullptr;

	CBase* pGameObject{ nullptr };

	if (ePrototype == PROTOTYPE::GAMEOBJECT)
		pGameObject = dynamic_cast<CGameObject*>(pBase)->Clone(pArg);
	else
		pGameObject = dynamic_cast<CComponent*>(pBase)->Clone(pArg);

	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

void CPrototype_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPrototypes[iLevelIndex])
		Safe_Release(Pair.second);
	
	m_pPrototypes[iLevelIndex].clear();
}

CBase* CPrototype_Manager::Find_Prototype(_uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag)
{
	auto it = m_pPrototypes[iPrototpyeLevelIndex].find(strPrototypeTag);

	if (it == m_pPrototypes[iPrototpyeLevelIndex].end())
		return nullptr;

	return it->second;
}

CPrototype_Manager* CPrototype_Manager::Create(_uint iNumLevels)
{
	CPrototype_Manager* pInstance = new CPrototype_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CPrototype_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPrototype_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);
		m_pPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);
}
