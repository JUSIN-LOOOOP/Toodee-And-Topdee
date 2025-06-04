#pragma once
#include "Base.h"

BEGIN(Engine)

class CSubjectObject;

class ENGINE_DLL CObserver abstract : public CBase
{
protected:
	CObserver();
	virtual ~CObserver() = default;

public:
	//Subject에게서 EVENT를 받습니다.
	//옵저버마다 EVENT 처리를 다르게 할 생각이라 가상함수로 뒀습니다.
	virtual void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) PURE;
	

	//Subject를 추가 합니다.
	HRESULT Add_Subjects(CSubjectObject* pSubject);
	
	//Subject가 옵저버를 해제할때 호출하는 함수 입니다.
	virtual void Remove_Subject(CSubjectObject* pSubject);

	// Subject들을 정리합니다.
	void Clear_Subjects();

protected:
	//Subject들에게 REPORT 를 전달합니다.
	virtual void Report(REPORT eReport, CSubjectObject* pSubject = nullptr);
	
protected:
	vector<CSubjectObject*> m_Subjects;

public:
	virtual void Free() override;
};

END