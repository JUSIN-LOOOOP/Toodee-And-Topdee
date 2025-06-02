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
	//Subject���Լ� EVENT�� �޽��ϴ�.
	//���������� EVENT ó���� �ٸ��� �� �����̶� �����Լ��� �׽��ϴ�.
	virtual void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) PURE;
	

	//Subject�� �߰� �մϴ�.
	HRESULT Add_Subjects(CSubjectObject* pSubject);
	
	//Subject�� �������� �����Ҷ� ȣ���ϴ� �Լ� �Դϴ�.
	virtual void Remove_Subject(CSubjectObject* pSubject);

	// Subject���� �����մϴ�.
	void Clear_Subjects();

protected:
	//Subject�鿡�� REPORT �� �����մϴ�.
	virtual void Report(REPORT eReport, CSubjectObject* pSubject = nullptr);
	
protected:
	vector<CSubjectObject*> m_Subjects;

public:
	virtual void Free() override;
};

END