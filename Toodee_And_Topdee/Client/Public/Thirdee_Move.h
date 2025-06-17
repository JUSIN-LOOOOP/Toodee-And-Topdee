#pragma once
#include "Thirdee_State.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CThirdee_Move final : public CThirdee_State
{
private:
	CThirdee_Move();
	virtual ~CThirdee_Move() = default;

public:
	HRESULT Initialize(PLAYERSTATE eState) override;
	void Enter(CPlayer_Thirdee* pPlayer) override;
	void HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta) override;
	void Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta) override;
	void Exit(CPlayer_Thirdee* pPlayer) override;

private:
	CGameInstance* m_pGameInstance = { nullptr };

	_float			m_fSoundDelay = {};
	_float			m_fSoundTime = {};

public:
	static CThirdee_Move* Create(PLAYERSTATE eState);
	virtual void Free() override;
};

END