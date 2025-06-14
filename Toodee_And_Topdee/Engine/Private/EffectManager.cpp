#include "EffectManager.h"
#include "PSystem.h"
#include "Collider.h"

#include "GameInstance.h"

CEffectManager::CEffectManager()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);

}

void CEffectManager::Update(_float fTimeDelta)
{
	for (auto& [tag, pSystem] : m_PSystems)
	{
		if (pSystem)
			pSystem->Update(fTimeDelta);
	}
}

HRESULT CEffectManager::Render()
{
	for (auto& [tag, pSystem] : m_PSystems)
	{
		if (pSystem)
			pSystem->Render();
	}
	return S_OK;
}

void CEffectManager::Stop_All()
{
	for (auto& [tag, pSystem] : m_PSystems)
	{
		if (pSystem)
			pSystem->Stop();
	}
}

HRESULT CEffectManager::Add_PSystem(CPSystem* pPSystem, const _wstring& strEffectTag)
{
	m_PSystems.emplace(strEffectTag, pPSystem);
	return S_OK;
}

HRESULT CEffectManager::Set_Active(const _wstring& strEffectTag, void* arg)
{
	CPSystem* pPSystem = Find_Pools(strEffectTag);

	if (nullptr == pPSystem)
		return E_FAIL;

	pPSystem->Reset(arg);

	return S_OK;
}

HRESULT CEffectManager::Set_Stop(const _wstring& strEffectTag, void* arg)
{
	CPSystem* pPSystem = Find_Pools(strEffectTag);
	
	pPSystem->Stop();
	
	return S_OK;
}

CPSystem* CEffectManager::Find_Pools(const _wstring strEffectTag)
{
	auto iter = m_PSystems.find(strEffectTag);
	if (iter == m_PSystems.end())
		return nullptr;

	return iter->second;
}

CEffectManager* CEffectManager::Create()
{
	return new CEffectManager;
}

void CEffectManager::Free()
{
	__super::Free();

	for (auto& PSystem : m_PSystems)
		Safe_Release(PSystem.second);

	m_PSystems.clear();

	Safe_Release(m_pGameInstance);

}
