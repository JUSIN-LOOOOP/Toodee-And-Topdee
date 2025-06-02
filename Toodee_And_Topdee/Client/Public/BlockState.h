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
	virtual void Enter(CInteractionBlock* pBlcok) PURE;														// State ���� �� State �ʱ�ȭ ����
	virtual void Update(CInteractionBlock* pBlcok, _float fTimeDelta) PURE;
	virtual void Exit(CInteractionBlock* pBlcok) PURE;														// State ���� �� ������

	void Request_ChangeState(CInteractionBlock* pBlcok, BLOCKSTATE eState);									// Player���� State ���� ��û

protected:
	BLOCKSTATE m_eState = {};

public:
	virtual void Free() override;
};

END