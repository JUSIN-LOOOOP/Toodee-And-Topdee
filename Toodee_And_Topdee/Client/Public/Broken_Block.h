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

class CBroken_Blocks final : public CPoolableObject
{

private:
	CBroken_Blocks(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBroken_Blocks(const CBroken_Blocks& Prototype);
	virtual ~CBroken_Blocks() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

	HRESULT				Initialize_Pool(void* pArg);

private:
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_float3				m_vPosition = { };

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CBroken_Blocks* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END