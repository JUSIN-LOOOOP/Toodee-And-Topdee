#pragma once
#include "Observer.h"

BEGIN(Engine)

class ENGINE_DLL CBreakTriggerObserver final :public CObserver
{
private:
	CBreakTriggerObserver();
	virtual ~CBreakTriggerObserver() = default;

public:
	virtual void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) override;

public:
	static CBreakTriggerObserver* Create();
	virtual void Free() override;

};

END