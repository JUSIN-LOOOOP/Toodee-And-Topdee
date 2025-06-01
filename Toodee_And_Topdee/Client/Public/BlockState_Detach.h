#pragma once
#include "BlockState.h"

BEGIN(Client)

class CBlockState_Detach final : public CBlockState
{
private:
	CBlockState_Detach();
	virtual ~CBlockState_Detach() = default;

public:
	virtual HRESULT Initialize(BLOCKSTATE eState) override;
	virtual void Enter(CBlock* pBlock) override;
	virtual void Update(CBlock* pBlock, _float fTimeDelta) override;
	virtual void Exit(CBlock* pBlock) override;

public:
	static CBlockState_Detach* Create(BLOCKSTATE eState);
	virtual void Free() override;
};

END