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


private:
	//Action มกวม
	_bool		m_bIsJumping = { false };
	_float		m_fCurrentJumpHeight = {};
	_float		m_fMaxJumpHeight = {};

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CPlayer_Toodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END