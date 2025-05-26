#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CBlock : public CGameObject
{
public:

private:
	CBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock(const CBlock& Prototype);
	virtual ~CBlock() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };

private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();


public:
	static CBlock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END