#pragma once
#include "Client_Defines.h"

#include "PoolableObject.h"
#include "Cannon.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CTransform;
class CTexture;
class CCollider;
END

BEGIN(Client)


class CFire_Projectile final : public CPoolableObject
{
public:
	typedef struct Protectile_Info {
		_float3						vPosition = { };
		CCannon::CANNON_DIRECTION	eDir = { };
	}PROTECTILE;

private:
	CFire_Projectile(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFire_Projectile(const CFire_Projectile& Prototype);
	virtual ~CFire_Projectile() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
	
	virtual HRESULT				Initialize_Pool(void* pArg) override;

private:
	CVIBuffer_Cube*				m_pVIBufferCom = { nullptr };
	CTransform*					m_pTransformCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	CCollider*					m_pColliderCom = { nullptr };

	CCannon::CANNON_DIRECTION	m_eDir = {};
	_uint						m_iMotionNum = {0};
	_float						m_fMotionIntervalTime = { 0.3f };
	_float						m_fAccumulateMotionTime = { 0.f };

	_float						m_fAccumulateLifeTime = { 0.f };
	_float						m_fLifeInterval = {8.f};

private:
	HRESULT						Ready_Components();
	void						SetUp_RenderState();
	void						Reset_RenderState();
	void						Fire_Forward(_float fTimeDelta);
	void						Change_Motion(_float fTimeDelta);


public:
	static CFire_Projectile* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*  Clone(void* pArg) override;
	virtual void Free() override;
};

END