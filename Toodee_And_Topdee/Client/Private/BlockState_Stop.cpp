#include "BlockState_Stop.h"

CBlockState_Stop::CBlockState_Stop()
{
}

HRESULT CBlockState_Stop::Initialize(BLOCKSTATE eState)
{
	if (ENUM_CLASS(eState) >= ENUM_CLASS(BLOCKSTATE::BLOCKSTATE_END))
		return E_FAIL;

	m_eState = eState;

	return S_OK;
}

void CBlockState_Stop::Enter(CInteractionBlock* pBlock)
{
}

void CBlockState_Stop::Update(CInteractionBlock* pBlock, _float fTimeDelta)
{
}

void CBlockState_Stop::Exit(CInteractionBlock* pBlock)
{
}

CBlockState_Stop* CBlockState_Stop::Create(BLOCKSTATE eState)
{
	CBlockState_Stop* pInstance = new CBlockState_Stop();

	if (FAILED(pInstance->Initialize(eState)))
	{
		MSG_BOX(TEXT("Failed Created : CBlockState_Stop"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlockState_Stop::Free()
{
	__super::Free();
}
