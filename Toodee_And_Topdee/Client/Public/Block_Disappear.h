#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Event.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN (Client)

class CBlock_Disappear final : public CGameObject
{
private:
	CBlock_Disappear(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Disappear(const CBlock_Disappear& Prototype);
	virtual ~CBlock_Disappear() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	void				Event_Pressed(const EVENT_REDBUTTON_PRESSED& event);
	void				Event_Unpressed(const EVENT_REDBUTTON_UNPRESSED& event);

private:
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom_Side = { nullptr };
	CTexture*			m_pTextureCom_Center = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

	_bool				m_bActivity = { true };
	_float3				m_vPosition = {}; 
	_float				m_fPositionX[3] = {};

	_uint				m_iPlayLevel = { };

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

private:
	HRESULT				Block_Render();
	HRESULT				Ready_SubscribeEvent(_uint iPlayerLevel);


public:
	static CBlock_Disappear* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END