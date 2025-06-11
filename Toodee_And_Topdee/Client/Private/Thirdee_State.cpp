#include "Thirdee_State.h"

CThirdee_State::CThirdee_State()
{
}

void CThirdee_State::Request_ChangeState(CPlayer_Thirdee* pPlayer, PLAYERSTATE eState)
{
	if (eState == m_eState)
		return;

	pPlayer->Change_State(eState);
}

void CThirdee_State::Free()
{
	__super::Free();
}
