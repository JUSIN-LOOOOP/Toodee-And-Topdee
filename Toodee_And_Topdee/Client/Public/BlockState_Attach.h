#pragma once
#include "BlockState.h"

BEGIN(Client)

class CBlockState_Attach final : public CBlockState
{
private:
	CBlockState_Attach();
	virtual ~CBlockState_Attach() = default;

public:
	virtual HRESULT Initialize(BLOCKSTATE eState) override;
	virtual void Enter(CInteractionBlock* pBlock) override;
	virtual void Update(CInteractionBlock* pBlock, _float fTimeDelta) override;
	virtual void Exit(CInteractionBlock* pBlock) override;

public:
	static CBlockState_Attach* Create(BLOCKSTATE eState);
	virtual void Free() override;

};

END