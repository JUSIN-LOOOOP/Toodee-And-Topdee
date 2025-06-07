#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Event.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CWater;

class CSink final : public CGameObject
{
private:
	CSink(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSink(const CSink& Prototype);
	virtual ~CSink() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void OnBlock();
	void OnOpen();
	void SinkBlock(const SINKBLOCKEVENT& Event);
	void SinkOpen(const SINKOPENEVENT& Event);

private:
	CCollider* m_pColliderCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CWater* m_pWater = { nullptr };
	
	_bool	m_bOnBlock = {};
	_uint	m_iPlayLevel = {};
	_float	m_fOpenHoleCount = {};
	_float	m_fTotalFillHeight = {};
	
private:
	HRESULT	Ready_Water();
	HRESULT Ready_Components();
	HRESULT Ready_SubscribeEvent(_uint iPlayLevel);

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CSink* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END