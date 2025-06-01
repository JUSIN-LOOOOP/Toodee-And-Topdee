#pragma once
#include "Client_Defines.h"

#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;

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

	CANNON_TYPE			m_eType {};
	_uint				m_iCannonDir = { };

	_float				m_fIntervalShooting = {0.f};
	_float				m_fAccumulateShootingTime = {0.f};

	_bool				m_bMotion = { false };
	_float				m_fIntervalMotion = { 0.f };
	_float				m_fAccumulateMotionTime = { 0.f };

	_float3				m_vOriginalPosition = {};


private:
	void				Shooting(_float fTimeDelta);
	void				Motion(_float fTimeDelta);
	
private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CCannon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END