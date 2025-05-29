#pragma once
#include "Observer.h"

BEGIN(Engine)

class ENGINE_DLL CClearTriggerObserver final :public CObserver
{
private:
	CClearTriggerObserver();
	virtual ~CClearTriggerObserver() = default;

public:
	void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) override;

private:
	unordered_set<CSubjectObject*> m_OverlapSubjects;

private:
	// Overlap Portal Subjects 추가, 삭제
	void Add_OverlapSubjects(CSubjectObject* pSubject);
	void Remove_OverlapSubjects(CSubjectObject* pSubject);
	
	void Clear_OverlapSubjects();

public:
	static CClearTriggerObserver* Create();
	virtual void Free();

};

END