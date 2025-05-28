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

	//State ȣ�� �Լ�
	virtual HRESULT Return_PrevState() override;	
	virtual void Move(_float fTimeDelta) override;	
	virtual void Action() override;					
	virtual void Stop() override;					

	//Observer ���� ���� REPORT ó��
	virtual void onReport(REPORT eReport) override;
private:
	//Action ���� ����
	JUMPSTATE	m_eJumpState = {};
	_float		m_fCurrentJumpPower = {};
	_float		m_fMaxJumpPower = {};
	_float		m_fGravityPower = {};


private:
	_uint KeyInput();	// Ű �Է� ����

	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_Observers();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
	void Action_Jump(_float fTimeDelta);	//CurrentState == Action State �϶� ����
public:
	static CPlayer_Toodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END