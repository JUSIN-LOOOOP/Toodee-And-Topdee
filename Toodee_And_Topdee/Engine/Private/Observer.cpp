#include "Observer.h"
#include "SubjectObject.h"


CObserver::CObserver()
{
}

HRESULT CObserver::Add_Subjects(CSubjectObject* pSubject)
{
	if (nullptr == pSubject)
		return E_FAIL;

	m_Subjects.push_back(pSubject);
	Safe_AddRef(pSubject);

	return S_OK;
}

void CObserver::Remove_Subject(CSubjectObject* pSubject)
{
	auto iter = m_Subjects.begin();

	for (iter; iter != m_Subjects.end();)
	{
		if (*iter == pSubject)
		{
			Safe_Release(*iter);
			iter = m_Subjects.erase(iter);
		}
		else
			iter++;
	}
}

void CObserver::Clear_Subjects()
{
	for (auto Subject : m_Subjects)
	{
		Subject->Remove_Observer(this);
		Safe_Release(Subject);
	}
}

void CObserver::Report(REPORT eReport)
{
	for (auto Subject : m_Subjects)
		Subject->onReport(eReport);
}

void CObserver::Free()
{
	__super::Free();

	

	for (auto iter = m_Subjects.begin() ; iter != m_Subjects.end();)
	{
		Safe_Release(*iter);
		iter = m_Subjects.erase(iter);
	}

}
