#pragma once
#include "Observer.h"

BEGIN(Engine)

class CClearTriggerObserver final :public CObserver
{
private:
	CClearTriggerObserver();
	virtual ~CClearTriggerObserver() = default;

public:
	void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) override;

private:
	void Add_OverlapSubjects(CSubjectObject* pSubject);
	void Subtract_OverlapSubjects(CSubjectObject* pSubject);
	void Clear_OverlapSubjects();
	unordered_set<CSubjectObject*> m_OverlapSubjects;
	
public:
	static CClearTriggerObserver* Create();
	virtual void Free();

};

END