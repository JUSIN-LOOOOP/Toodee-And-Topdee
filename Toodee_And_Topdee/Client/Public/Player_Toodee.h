#pragma once

#include "Player.h"

BEGIN(Client)

class CPlayer_Toodee final : public CPlayer
{
private:
	enum JUMPSTATE { JS_JUMPING, JS_HANGSTART, JS_HANGING, JS_HANGEND, JS_FALLING};
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


private:
	//Action มกวม
	JUMPSTATE	m_eJumpState = {};
	_bool		m_bIsJumping = { false };
	_float		m_fCurrentJumpPower = {};
	_float		m_fMaxJumpPower = {};
	_float		m_fGravityPower = {};
	_float		m_fHangTime= {};
	_float		m_fHangDelay = {};
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
	void Action_Jump(_float fTimeDelta);
public:
	static CPlayer_Toodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END