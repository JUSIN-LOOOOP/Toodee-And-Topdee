#pragma once
#include "Base.h"
#include <stack>

BEGIN(Engine)

class CPool final : public CBase
{
public:
	CPool();
	virtual ~CPool() = default;

public:
	void						Push(class CPoolableObject* pGameObject);
	class CPoolableObject*		Pop();

private:
	stack<class CPoolableObject*> m_poolStack;

public:
	static CPool* Create();
	virtual void Free() override;

};

END