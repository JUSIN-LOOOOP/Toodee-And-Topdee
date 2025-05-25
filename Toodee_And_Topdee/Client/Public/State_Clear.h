#pragma once
#include "PlayerState.h"

class CState_Clear final : public CPlayerState
{
private:
	CState_Clear();
	virtual ~CState_Clear() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(CPlayer* pPlayer) override;
	virtual void HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta) override;
	virtual void Update(CPlayer* pPlayer, _float fTimeDelta) override;
	virtual void Exit(CPlayer* pPlayer) override;

private:
	virtual void UpdateAnim(_float fTimeDelta) override;

public:
	static CState_Clear* Create(void* pArg);
	virtual void Free();
};

