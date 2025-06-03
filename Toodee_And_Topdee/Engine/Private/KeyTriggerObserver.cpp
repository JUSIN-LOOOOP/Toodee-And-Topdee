#include "KeyTriggerObserver.h"

CKeyTriggerObserver::CKeyTriggerObserver()
{
}


void CKeyTriggerObserver::onNotify(EVENT eEvent, CSubjectObject* pSubject)
{
	switch (eEvent)
	{
	case EVENT::ADD_KEY:
		m_iTotalKeyCount++;
		break;

	case EVENT::GET_KEY:
		m_iCurrentKeyCount++;
		break;
	}

	if (m_iCurrentKeyCount >= m_iTotalKeyCount)
		__super::Report(REPORT::OPEN_KEYBLOCK);
}

CKeyTriggerObserver* CKeyTriggerObserver::Create()
{
	return new CKeyTriggerObserver();
}

void CKeyTriggerObserver::Free()
{
	__super::Free();
}
