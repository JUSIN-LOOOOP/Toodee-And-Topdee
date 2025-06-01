#include "BlockState_Push.h"

CBlockState_Push::CBlockState_Push()
{
}

HRESULT CBlockState_Push::Initialize(BLOCKSTATE eState)
{
	if (ENUM_CLASS(eState) >= ENUM_CLASS(BLOCKSTATE::BLOCKSTATE_END))
		return E_FAIL;

	m_eState = eState;

	return S_OK;
}

void CBlockState_Push::Enter(CBlock* pBlock)
{
}

void CBlockState_Push::Update(CBlock* pBlock, _float fTimeDelta)
{
	if (pBlock->Pushed(fTimeDelta))
		pBlock->Change_State(BLOCKSTATE::STOP);
}

void CBlockState_Push::Exit(CBlock* pBlock)
{
}

CBlockState_Push* CBlockState_Push::Create(BLOCKSTATE eState)
{
	CBlockState_Push* pInstance = new CBlockState_Push();

	if (FAILED(pInstance->Initialize(eState)))
	{
		MSG_BOX(TEXT("Failed Created : CBlockState_Push"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlockState_Push::Free()
{
	__super::Free();
}
