#pragma once
#include "Observer.h"

BEGIN(Engine)

class ENGINE_DLL CKeyTriggerObserver final : public CObserver
{
private:
	CKeyTriggerObserver();
	virtual ~CKeyTriggerObserver() = default;

public:
	virtual void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) override;

private:
	_uint m_iTotalKeyCount = { 0 };
	_uint m_iCurrentKeyCount = { 0 };

public:
	static CKeyTriggerObserver* Create();
	virtual void Free() override;
};

END