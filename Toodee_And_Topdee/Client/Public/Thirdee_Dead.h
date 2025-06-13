#pragma once
#include "Thirdee_State.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CThirdee_Dead final : public CThirdee_State
{
private:
	CThirdee_Dead();
	virtual ~CThirdee_Dead() = default;

public:
	HRESULT Initialize(PLAYERSTATE eState) override;
	void Enter(CPlayer_Thirdee* pPlayer) override;
	void HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta) override;
	void Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta) override;
	void Exit(CPlayer_Thirdee* pPlayer) override;

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CThirdee_Dead* Create(PLAYERSTATE eState);
	virtual void Free() override;
};

END