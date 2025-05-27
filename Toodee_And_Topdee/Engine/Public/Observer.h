#pragma once
#include "Base.h"

BEGIN(Engine)

class CSubjectObject;

class CObserver abstract : public CBase
{
public:
	enum class EVENT { ENTER_PORTAL, EXIT_PORTAL };

protected:
	CObserver();
	virtual ~CObserver() = default;

public:
	virtual void onNotify(EVENT eEvent, CSubjectObject* pSubject = nullptr) PURE;
	HRESULT Add_Subjects(CSubjectObject* pSubject);
	
protected:
	vector<CSubjectObject*> m_Subjects;

public:
	virtual void Free() override;
};

END