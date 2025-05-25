#pragma once
#include "PlayerState.h"
class CState_Action final : public CPlayerState
{
private:
	CState_Action();
	virtual ~CState_Action() = default;

public:
	HRESULT Initialize(void* pArg) override;
	void Enter(CPlayer* pPlayer) override;
	void HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta) override;
	void Update(CPlayer* pPlayer, _float fTimeDelta) override;
	void Exit(CPlayer* pPlayer) override;
	void UpdateAnim(_float fTimeDelta) override;


public:
	static CState_Action* Create(void* pArg);
	virtual void Free() override;
};

