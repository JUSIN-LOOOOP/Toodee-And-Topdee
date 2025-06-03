#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "InteractionBlock.h"

BEGIN(Client)

class CInteractionBlock;

class CBlockState abstract : public CBase
{
protected:
	CBlockState();
	virtual ~CBlockState() = default;

public:
	virtual HRESULT Initialize(BLOCKSTATE eState) PURE;
	virtual void Enter(CInteractionBlock* pBlcok) PURE;														// State 변경 시 State 초기화 목적
	virtual void Update(CInteractionBlock* pBlcok, _float fTimeDelta) PURE;
	virtual void Exit(CInteractionBlock* pBlcok) PURE;														// State 변경 전 마지막

	void Request_ChangeState(CInteractionBlock* pBlcok, BLOCKSTATE eState);									// Player에서 State 변경 요청

protected:
	BLOCKSTATE m_eState = {};

public:
	virtual void Free() override;
};

END