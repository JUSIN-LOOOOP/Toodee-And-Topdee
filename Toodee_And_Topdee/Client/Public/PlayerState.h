#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)

class CPlayerState abstract : public CBase
{
public:

protected:
	CPlayerState();
	virtual ~CPlayerState() = default;

public:
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void Enter(CPlayer* pPlayer) PURE;														// State ���� �� State �ʱ�ȭ ����
	virtual void HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta) PURE;			// Player Key Input
	virtual void Update(CPlayer* pPlayer, _float fTimeDelta) PURE;									
	virtual void Exit(CPlayer* pPlayer) PURE;														// State ���� �� ������
	virtual void UpdateAnim(_float fTimeDelta) PURE;												// AnimCount Update

	void Request_ChangeState(CPlayer* pPlayer, PLAYERSTATE eState);									// Player���� State ���� ��û

public:
	PLAYERSTATE GetTextureKey() {																	
		return m_eState;
	}

	_uint GetAnimCount() {																			
		return m_iCurrentAnimCount;
	}

	_uint GetMaxAnimCount() {																		
		return m_iMaxAnimCount;
	}


protected:
	PLAYERSTATE				m_eState = {};															// Player State													// 
	_uint					m_iMaxAnimCount = {};
	_uint					m_iCurrentAnimCount = {};
	_float					m_fAnimDelay = {};
	_float					m_fAnimTime = {};

public:
	virtual void Free() override;
};

END