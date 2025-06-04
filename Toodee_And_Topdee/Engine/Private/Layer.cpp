#include "Layer.h"
#include "GameObject.h"
#include "Collider.h"

#include "GameInstance.h"

CLayer::CLayer()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);

}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for(auto iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		if ((*iter)->IsDead())
		{
			CComponent* pComponent = (*iter)->Get_Component(TEXT("Com_Collision"));
			if (pComponent != nullptr)
				m_pGameInstance->Delete_Collider(m_pGameInstance->Get_CurrentLevelID(), reinterpret_cast<CCollider**>(&pComponent));

			Safe_Release(*iter);
			iter = m_GameObjects.erase(iter);
		}
		else 
		{
			(*iter)->Priority_Update(fTimeDelta);
			++iter;
		}
		
	}
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Update(fTimeDelta);
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);
	}
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_Component(strComponentTag);
}

CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();

	Safe_Release(m_pGameInstance);

}
