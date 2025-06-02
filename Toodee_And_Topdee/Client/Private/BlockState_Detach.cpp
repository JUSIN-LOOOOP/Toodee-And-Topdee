#include "BlockState_Detach.h"

CBlockState_Detach::CBlockState_Detach()
{
}

HRESULT CBlockState_Detach::Initialize(BLOCKSTATE eState)
{
	if (ENUM_CLASS(eState) >= ENUM_CLASS(BLOCKSTATE::BLOCKSTATE_END))
		return E_FAIL;

	m_eState = eState;

	return S_OK;
}

void CBlockState_Detach::Enter(CInteractionBlock* pBlock)
{
}

void CBlockState_Detach::Update(CInteractionBlock* pBlock, _float fTimeDelta)
{
	if (pBlock->Detached(fTimeDelta))
		pBlock->Change_State(BLOCKSTATE::STOP);
}

void CBlockState_Detach::Exit(CInteractionBlock* pBlock)
{
}

CBlockState_Detach* CBlockState_Detach::Create(BLOCKSTATE eState)
{
	CBlockState_Detach* pInstance = new CBlockState_Detach();

	if (FAILED(pInstance->Initialize(eState)))
	{
		MSG_BOX(TEXT("Failed Created : CBlockState_Detach"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlockState_Detach::Free()
{
	__super::Free();
}
