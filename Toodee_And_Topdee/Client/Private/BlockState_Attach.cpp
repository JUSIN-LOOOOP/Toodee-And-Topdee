#include "BlockState_Attach.h"

CBlockState_Attach::CBlockState_Attach()
{
}


HRESULT CBlockState_Attach::Initialize(BLOCKSTATE eState)
{
	if (ENUM_CLASS(eState) >= ENUM_CLASS(BLOCKSTATE::BLOCKSTATE_END))
		return E_FAIL;

	m_eState = eState;

	return S_OK;
}

void CBlockState_Attach::Enter(CBlock* pBlock)
{
}

void CBlockState_Attach::Update(CBlock* pBlock, _float fTimeDelta)
{
	if (false == pBlock->IsAttach())
		pBlock->Attached(fTimeDelta);
	else
		pBlock->Update_Attached_Position();
}

void CBlockState_Attach::Exit(CBlock* pBlock)
{
}

CBlockState_Attach* CBlockState_Attach::Create(BLOCKSTATE eState)
{
	CBlockState_Attach* pInstance = new CBlockState_Attach();

	if (FAILED(pInstance->Initialize(eState)))
	{
		MSG_BOX(TEXT("Failed Created : CBlockState_Attach"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlockState_Attach::Free()
{
	__super::Free();
}
