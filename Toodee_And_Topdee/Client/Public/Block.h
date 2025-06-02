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

class CBlock abstract : public CGameObject
{
protected:
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

	_float ComputeDirDotLook(const _float3& vPlayerPosition, const _float3& vLook);
	
protected:
	/* Component */
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

protected:
	virtual	HRESULT Ready_Components();
	void			SetUp_BlockInfo(void* pArg);
	virtual void	SetUp_RenderState();
	virtual void	Reset_RenderState();

	_uint		m_TextureIdx = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END