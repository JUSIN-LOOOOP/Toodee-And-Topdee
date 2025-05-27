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

	pSubject->Add_Observer(this);

	return S_OK;
}

void CObserver::Free()
{
	__super::Free();

	auto iter = m_Subjects.begin();

	for (iter; iter != m_Subjects.end();)
	{
		Safe_Release(*iter);
		iter = m_Subjects.erase(iter);
	}

}
