#pragma once
#include "PlayerState.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CState_Move final : public CPlayerState
{
private:
	CState_Move();
	virtual ~CState_Move() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Enter(CPlayer* pPlayer) override;
	virtual void HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta) override;
	virtual void Update(CPlayer* pPlayer, _float fTimeDelta) override;
	virtual void Exit(CPlayer* pPlayer) override;
	virtual void UpdateAnim(_float fTimeDelta) override;

private:
	CGameInstance* m_pGameInstance = { nullptr };

	_float			m_fSoundDelay = {};
	_float			m_fSoundTime = {};

public:
	static CState_Move* Create(void* pArg);
	virtual void Free() override;
};

END