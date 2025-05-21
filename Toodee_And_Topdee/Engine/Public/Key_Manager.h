#pragma once
#include "Base.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	void			Update();

public:
	_bool			Key_Pressing(int _iKey);
	_bool			Key_Up(int _iKey);
	_bool			Key_Down(int _iKey);

private:
	bool m_bKeyState[VK_MAX] = {};
	bool m_bPrevKeyState[VK_MAX] = {};

	vector<_int> keysToCheck = {};

private:
	HRESULT			Initialize();


public:
	static CKey_Manager* Create();
	virtual void Free() override;
};

END