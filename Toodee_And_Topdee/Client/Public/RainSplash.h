#pragma once
#include "Client_Defines.h"
#include "PoolableObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider;

END

BEGIN(Client)
class CRainSplash final : public CPoolableObject
{
private:
	CRainSplash(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRainSplash(const CRainSplash& Prototype);
	virtual ~CRainSplash() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	HRESULT				Initialize_Pool(void* pArg) override;


private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTransform*					m_pTransformCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

	_float3						m_vStartPosition = {};

	_uint						m_iMotionNum = { 1 };
	_float						m_fMotionIntervalTime = { 1.f };
	_float						m_fAccumulateMotionTime = { 0.f };

	_uint						m_iLevel = {};
private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	void				Change_Motion(_float fTimeDelta);

public:
	static CRainSplash* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;



	

};

END