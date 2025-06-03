#include "BreakTriggerObserver.h"

CBreakTriggerObserver::CBreakTriggerObserver()
{
}

void CBreakTriggerObserver::onNotify(EVENT eEvent, CSubjectObject* pSubject)
{
	switch (eEvent)
	{
	case EVENT::BLOCK_BREAK:
		__super::Report(REPORT::BLOCK_BREAK, pSubject);
		break;
	}
}


CBreakTriggerObserver* CBreakTriggerObserver::Create()
{
	return new CBreakTriggerObserver();
}

void CBreakTriggerObserver::Free()
{
	__super::Free();
}
