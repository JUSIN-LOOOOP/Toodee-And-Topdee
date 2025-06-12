#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Player_Thirdee.h"

BEGIN(Client)

class CThirdee_State abstract : public CBase
{
protected:
	CThirdee_State();
	virtual ~CThirdee_State() = default;

public:
	virtual HRESULT Initialize(PLAYERSTATE eState) PURE;
	virtual void Enter(CPlayer_Thirdee* pPlayer) PURE;														
	virtual void HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta) PURE;			
	virtual void Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta) PURE;
	virtual void Exit(CPlayer_Thirdee* pPlayer) PURE;																									
	void Request_ChangeState(CPlayer_Thirdee* pPlayer, PLAYERSTATE eState);									

protected:
	PLAYERSTATE				m_eState = {};

public:
	virtual void Free() override;
};

END