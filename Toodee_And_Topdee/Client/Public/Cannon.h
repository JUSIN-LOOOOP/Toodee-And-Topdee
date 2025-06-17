#pragma once
#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CCannon final : public CGameObject
{
public:
	/* 쏘는 방향 기준 */
	enum class CANNON_DIRECTION { RGIHT, LEFT, UP, DOWN, NONE };
	enum class CANNON_TYPE { FIRE, LASER, NONE };

	struct CANNON_INFO
	{
		_float3		vPosition = {};
		CANNON_DIRECTION eDir;
		CANNON_TYPE  eType;
	};

private:
	CCannon(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCannon(const CCannon& Prototype);
	virtual ~CCannon() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

private:
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CTexture*			m_pTextureCom_Eff = { nullptr };


	CANNON_TYPE			m_eType {};
	_uint				m_iCannonDir = { };

	_float				m_fIntervalShooting = { 0.4f };
	_float				m_fAccumulateShootingTime = {0.f};

	_bool				m_bMotion = { false };
	_float				m_fIntervalMotion = { 0.3f };
	_float				m_fAccumulateMotionTime = { 0.f };

	_float3				m_vOriginalPosition = {};

	//effect
	_bool				m_bEffect = { false };
	_uint				m_iEffNum = { 0 };
	_float				m_fEffIntervalMotion = { 0.03f };
	_float				m_fEffAccumurateTime = {0.f};
	
	_uint				m_iLevel = {};


private:
	void				Shooting(_float fTimeDelta);
	void				Motion(_float fTimeDelta);
	void				Effect_Motion(_float fTimeDelta);
	
private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	void				SetUp_AlphaRenderState();
	void				Reset_AlphaRenderState();

public:
	static CCannon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END