#pragma once

#include "Base.h"

BEGIN(Engine)

class CObserver;
class CGameObject;

class ENGINE_DLL CSubjectObject abstract : public virtual CBase
{
protected:
	CSubjectObject();
	CSubjectObject(const CSubjectObject& Prototype);
	virtual ~CSubjectObject() = default;

public:
	//Observer 에게서 REPORT를 받습니다.
	//객체마다 REPORT 처리를 다르게 할 생각이라 가상함수로 뒀습니다.
	virtual void onReport(REPORT eReport, CSubjectObject* pSubject = nullptr) PURE;
	
	//옵저버 추가
	HRESULT Add_Observer(CObserver* pObserver);
	
	//옵저버 삭제
	void Remove_Observer(CObserver* pObserver);

protected:
	vector<CObserver*> m_Observers;

protected:
	//Observer 들에게 EVENT를 전달합니다.
	void Notify(EVENT eEvent);
	void Notify_Dead();

public:
	void SubjectFree();
};

END