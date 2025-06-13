#pragma once
#include "Client_Defines.h"

#include "GameObject.h"
#include "Event.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CFinalBoss final : public CGameObject
{

private:
	CFinalBoss(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFinalBoss(const CFinalBoss& Prototype);
	virtual ~CFinalBoss() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	void				Event_FinalBoss_Attack(const FIANLBOSSATTACK_EVENT& event);

private:
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom_Idle = { nullptr };
	CTexture*			m_pTextureCom_Attack = { nullptr };
	CTexture*			m_pTextureCom_Effect = { nullptr };
	CTexture*			m_pTextureCom_Projectile = {nullptr};


	_float3				m_vBodyPosition = {};
	_float3				m_vAttackEffectPositionOffset = {};

	_bool				m_IsAttacking = { false };
	_bool				m_IsAttackEffect = { false };

	/* always */
	_float				m_fBodyIntervalTime = { 1.5f };
	_float				m_fBodyAccumurateTime = { 0.f };
	_bool				m_bBodyTurn = { false };

	_float				m_vUpOffest = { 1.f };

	/* Attacking */
	_float				m_fAttackIntervalTime = { 0.1f };
	_float				m_fAttackAccumurateTime = { 0.f };
	_uint				m_iAttackNum = { 0 };
	_bool				m_bAttackTurn = { true };

	_float				m_fAttackShakingIntervalTime = { 0.15f };
	_float				m_fAttackShakingAccumurateTime = { 0.f };
	_float3				m_vAttackShakingPosition = {0.f,0.f,0.f};
	_float3				m_vAttackShakingOffestPosition = { 0.f,0.3f,0.f };
	_bool				m_bAttackShakingTurn = { false };


	/* Once Attack - Effect*/
	_float				m_fEffectIntervalTime = { 0.16f };
	_float				m_fEffectAccumurateTime = { 0.f };
	_uint				m_iEffectNum = { 0 };

	_float3				m_vEffectProjectilePosition = {};
	_float				m_fEffectProjectilePositionX = {};

private:
	void				Motion_body(_float fTimeDelta);
	void				Motion_Attack(_float fTimeDelta);
	void				Motion_Effect(_float fTimeDelta);

private:
	HRESULT				Ready_SubscribeEvent(_uint iPlayerLevel);
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CFinalBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END