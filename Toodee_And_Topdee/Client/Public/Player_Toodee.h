#pragma once

#include "Player.h"

BEGIN(Client)

class CPlayer_Toodee final : public CPlayer
{


private:
	CPlayer_Toodee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_Toodee(const CPlayer_Toodee& Prototype);
	virtual ~CPlayer_Toodee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	//State 호출 함수
	virtual HRESULT Return_PrevState() override;	
	virtual void Idle() override;
	virtual void Move(_float fTimeDelta) override;	
	virtual void Action() override;					
	virtual void Stop() override;					
	virtual void Clear() override;
	//Observer 에서 받은 REPORT 처리
	virtual void onReport(REPORT eReport) override;
private:
	//Action 점프 변수
	JUMPSTATE	m_eJumpState = {};
	_float		m_fCurrentJumpPower = {};
	_float		m_fAccumulationJumpPower = {};
	_float		m_fIncreaseJumpPower = {};
	_float		m_fMaxIncreaseJumpPower = {};
	_float		m_fGravityPower = {};
	_bool		m_bFalling = {};


private:
	_uint KeyInput();	// 키 입력 저장

	void Action_Jump(_float fTimeDelta);	//CurrentState == Action State 일때 실행
	void Gravity(_float fTimeDelta);
	void Compute_Gravity(_float fTimeDelta);

	void Check_CollisionState();

;	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_Observers();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CPlayer_Toodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END