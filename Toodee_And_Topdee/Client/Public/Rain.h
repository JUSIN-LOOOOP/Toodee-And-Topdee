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
class CRain final : public CPoolableObject
{
public:
	typedef struct Rain_Position_Info {
		_float3				vStartPosition = { };
		_float3				vCrashPosition = { };
	}RAIN;

private:
	CRain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRain(const CRain& Prototype);
	virtual ~CRain() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float3				m_vStartPosition = {};
	_float3				m_vCrashPosition = {};

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CRain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;



	

};

END