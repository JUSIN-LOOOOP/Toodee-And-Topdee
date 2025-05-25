#pragma once
#include "PlayerState.h"

class CState_Stop final : public CPlayerState
{
private:
	CState_Stop();
	virtual ~CState_Stop() = default;

public:
	HRESULT Initialize(void* pArg) override;
	void Enter(CPlayer* pPlayer) override;
	void HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta) override;
	void Update(CPlayer* pPlayer, _float fTimeDelta) override;
	void Exit(CPlayer* pPlayer) override;
	void UpdateAnim(_float fTimeDelta) override;


public:
	static CState_Stop* Create(void* pArg);
	virtual void Free() override;

};

