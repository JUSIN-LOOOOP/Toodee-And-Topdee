#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Block.h"

BEGIN(Client)

class CBlock;

class CBlockState abstract : public CBase
{
protected:
	CBlockState();
	virtual ~CBlockState() = default;

public:
	virtual HRESULT Initialize(BLOCKSTATE eState) PURE;
	virtual void Enter(CBlock* pBlcok) PURE;														// State 변경 시 State 초기화 목적
	virtual void Update(CBlock* pBlcok, _float fTimeDelta) PURE;
	virtual void Exit(CBlock* pBlcok) PURE;														// State 변경 전 마지막

	void Request_ChangeState(CBlock* pBlcok, BLOCKSTATE eState);									// Player에서 State 변경 요청

protected:
	BLOCKSTATE m_eState = {};

public:
	virtual void Free() override;
};

END