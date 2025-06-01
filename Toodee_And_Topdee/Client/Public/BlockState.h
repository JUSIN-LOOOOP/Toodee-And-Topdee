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
	virtual void Enter(CBlock* pBlcok) PURE;														// State ���� �� State �ʱ�ȭ ����
	virtual void Update(CBlock* pBlcok, _float fTimeDelta) PURE;
	virtual void Exit(CBlock* pBlcok) PURE;														// State ���� �� ������

	void Request_ChangeState(CBlock* pBlcok, BLOCKSTATE eState);									// Player���� State ���� ��û

protected:
	BLOCKSTATE m_eState = {};

public:
	virtual void Free() override;
};

END