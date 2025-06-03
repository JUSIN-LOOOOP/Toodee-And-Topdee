#include "Pool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "PoolableObject.h"

CPool::CPool()
{
}

void CPool::Push(CPoolableObject* pGameObject)
{	
	m_poolStack.push(pGameObject);

	Safe_AddRef(pGameObject);
}

CPoolableObject* CPool::Pop()
{
	if (m_poolStack.empty())
		return nullptr;

	CPoolableObject* pGameObject = m_poolStack.top();
	Safe_Release(pGameObject);
	m_poolStack.pop();

	return pGameObject;
}

CPool* CPool::Create()
{
	return  new CPool;
}

void CPool::Free()
{
	__super::Free();

	while (!m_poolStack.empty())
	{
		Safe_Release(m_poolStack.top());
		m_poolStack.pop();
	}

}
