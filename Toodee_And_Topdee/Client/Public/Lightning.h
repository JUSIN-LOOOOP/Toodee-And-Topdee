#pragma once
#include "Client_Defines.h"
#include "PoolableObject.h"


BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;

END

BEGIN(Client)

class CLightning : public CPoolableObject
{
public:
	typedef struct Lightining_Position_Info {
		_float3				vStartPosition = { };
		_float3				vCrashPosition = { };
	}LIGHTNING;

private:
	CLightning(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLightning(const CLightning& Prototype);
	virtual ~CLightning() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

	HRESULT				Initialize_Pool(void* pArg);


private:
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

	_uint				m_iMotionNumber = {};
	_float				m_fIntervalMotion = {};
	_float				m_fAccumulateMotion = {};

private:
	void				Motion(_float fTimeDelta);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CLightning* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END