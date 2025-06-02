#include "BlockState.h"


CBlockState::CBlockState()
{
}

void CBlockState::Enter(CInteractionBlock* pBlcok)
{
}

void CBlockState::Update(CInteractionBlock* pBlcok, _float fTimeDelta)
{
}

void CBlockState::Exit(CInteractionBlock* pBlcok)
{
}

void CBlockState::Request_ChangeState(CInteractionBlock* pBlcok, BLOCKSTATE eState)
{
	if (eState == m_eState)
		return;

	pBlcok->Change_State(eState);

}

void CBlockState::Free()
{
	__super::Free();
}
