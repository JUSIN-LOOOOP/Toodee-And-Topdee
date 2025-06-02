#include "SubjectObject.h"
#include "Observer.h"

CSubjectObject::CSubjectObject()
{
}

CSubjectObject::CSubjectObject(const CSubjectObject& Prototype)
{

}

HRESULT CSubjectObject::Add_Observer(CObserver* pObserver)
{
	if (nullptr == pObserver)
		return E_FAIL;

	m_Observers.push_back(pObserver);
	Safe_AddRef(pObserver);
	
	return S_OK;
}

void CSubjectObject::Remove_Observer(CObserver* pObserver)
{
	auto iter = m_Observers.begin();

	for (iter; iter != m_Observers.end();)
	{
		if (*iter == pObserver)
		{
			Safe_Release(*iter);
			iter = m_Observers.erase(iter);
		}
		else
			iter++;
	}
}

void CSubjectObject::Notify(EVENT eEvent)
{
	for (auto Observer : m_Observers)
		Observer->onNotify(eEvent, this);
}

void CSubjectObject::Notify_Dead()
{
	for (auto Observer : m_Observers)
		Observer->Remove_Subject(this);
}

void CSubjectObject::Free()
{
	auto iter = m_Observers.begin();

	for (iter; iter != m_Observers.end();)
	{
		Safe_Release(*iter);
		iter = m_Observers.erase(iter);
	}
}
