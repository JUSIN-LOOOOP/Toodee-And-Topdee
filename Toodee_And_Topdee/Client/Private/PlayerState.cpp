#include "PlayerState.h"

CPlayerState::CPlayerState()
{
}


void CPlayerState::Free()
{
	__super::Free();
}

void CPlayerState::Request_ChangeState(CPlayer* pPlayer, PLAYERSTATE eState)
{
	if (eState == m_eState)
		return;

	pPlayer->Change_State(eState);
}
