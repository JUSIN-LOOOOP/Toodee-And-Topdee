#pragma once
#include "Thirdee_State.h"

BEGIN(Client)

class CThirdee_Idle final : public CThirdee_State
{
private:
	CThirdee_Idle();
	virtual ~CThirdee_Idle() = default;

public:
	HRESULT Initialize(PLAYERSTATE eState) override;
	void Enter(CPlayer_Thirdee* pPlayer) override;
	void HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta) override;
	void Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta) override;
	void Exit(CPlayer_Thirdee* pPlayer) override;

public:
	static CThirdee_Idle* Create(PLAYERSTATE eState);
	virtual void Free() override;
};

END