#pragma once

#include "PlayerState.h"

BEGIN(Client)

class CState_Idle final : public CPlayerState
{
private:
	CState_Idle();
	virtual ~CState_Idle() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(CPlayer* pPlayer) override;
	virtual void HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta) override;
	virtual void Update(CPlayer* pPlayer, _float fTimeDelta) override;
	virtual void Exit(CPlayer* pPlayer) override;
	virtual void UpdateAnim(_float fTimeDelta) override;

public:
	static CState_Idle* Create(void* pArg);
	virtual void Free() override;
};

END