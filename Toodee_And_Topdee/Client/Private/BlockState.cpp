#include "BlockState.h"


CBlockState::CBlockState()
{
}

void CBlockState::Enter(CBlock* pBlcok)
{
}

void CBlockState::Update(CBlock* pBlcok, _float fTimeDelta)
{
}

void CBlockState::Exit(CBlock* pBlcok)
{
}

void CBlockState::Request_ChangeState(CBlock* pBlcok, BLOCKSTATE eState)
{
	if (eState == m_eState)
		return;

	pBlcok->Change_State(eState);

}

void CBlockState::Free()
{
	__super::Free();
}
