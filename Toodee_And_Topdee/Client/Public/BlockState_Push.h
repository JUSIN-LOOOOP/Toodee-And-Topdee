#pragma once
#include "BlockState.h"

BEGIN(Client)

class CBlockState_Push final : public CBlockState
{
private:
	CBlockState_Push();
	virtual ~CBlockState_Push() = default;

public:
	virtual HRESULT Initialize(BLOCKSTATE eState) override;
	virtual void Enter(CInteractionBlock* pBlock) override;
	virtual void Update(CInteractionBlock* pBlock, _float fTimeDelta) override;
	virtual void Exit(CInteractionBlock* pBlock) override;

public:
	static CBlockState_Push* Create(BLOCKSTATE eState);
	virtual void Free() override;
};

END