#include "ClearTriggerObserver.h"
#include "SubjectObject.h"

CClearTriggerObserver::CClearTriggerObserver()
{
}

void CClearTriggerObserver::onNotify(EVENT eEvent, CSubjectObject* pSubject)
{
	switch (eEvent)
	{
	case EVENT::ENTER_PORTAL:
		Add_OverlapSubjects(pSubject);
		break;
		
	case EVENT::EXIT_PORTAL:
		Remove_OverlapSubjects(pSubject);
		break;
	}

	if (m_OverlapSubjects.size() >= 2) // 포탈에 2명 다 들어왔다면
	{
		// Subjects 들에게 CanClear 전달
		__super::Report(REPORT::REPORT_CANCLEAR);
	}
}

void CClearTriggerObserver::Remove_Subject(CSubjectObject* pSubject)
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

	if (nullptr == pSubject)
		return;

	_uint iErase = m_OverlapSubjects.erase(pSubject);

	if (iErase == 0)
		return;

	Safe_Release(pSubject);
}

void CClearTriggerObserver::Add_OverlapSubjects(CSubjectObject* pSubject)
{
	if (nullptr == pSubject)
		return;
	
	pair<unordered_set<CSubjectObject*>::iterator, bool> ResultPair;
	ResultPair = m_OverlapSubjects.insert(pSubject);

	if (false == ResultPair.second)
		return;

	Safe_AddRef(pSubject);
}

void CClearTriggerObserver::Remove_OverlapSubjects(CSubjectObject* pSubject)
{
	if (nullptr == pSubject)
		return;

	_uint iErase = m_OverlapSubjects.erase(pSubject);

	if (iErase == 0)
		return;

	Safe_Release(pSubject);
}

void CClearTriggerObserver::Clear_OverlapSubjects()
{
	for (auto Subjects : m_OverlapSubjects)
		Safe_Release(Subjects);

	m_OverlapSubjects.clear();
}

CClearTriggerObserver* CClearTriggerObserver::Create()
{
	return new CClearTriggerObserver();
}

void CClearTriggerObserver::Free()
{
	__super::Free();

	Clear_OverlapSubjects();
}
