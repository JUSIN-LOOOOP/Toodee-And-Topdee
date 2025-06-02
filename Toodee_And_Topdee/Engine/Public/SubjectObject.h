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
	//Observer ���Լ� REPORT�� �޽��ϴ�.
	//��ü���� REPORT ó���� �ٸ��� �� �����̶� �����Լ��� �׽��ϴ�.
	virtual void onReport(REPORT eReport, CSubjectObject* pSubject = nullptr) PURE;
	
	//������ �߰�
	HRESULT Add_Observer(CObserver* pObserver);
	
	//������ ����
	void Remove_Observer(CObserver* pObserver);

protected:
	vector<CObserver*> m_Observers;

protected:
	//Observer �鿡�� EVENT�� �����մϴ�.
	void Notify(EVENT eEvent);
	void Notify_Dead();

public:
	void SubjectFree();
};

END