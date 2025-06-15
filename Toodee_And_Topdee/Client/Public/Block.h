#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CVIBuffer_DiffuseCube;
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
	
	/* Shadow*/
protected:
	CVIBuffer_DiffuseCube* m_VIBufferCom_Diffuse = { nullptr };
	_float4x4				m_matrixShadow = {};

	/* Shadow : switching - camera angle */
	_float					m_fToodeeAngle = { 390.f };
	_float					m_fTopdeeAngle = { 210.f };
	_float					m_fCurAngle = { };

	_float					m_fToodeeLightDirY = { 1.8f };
	_float					m_fTopdeeLightDirY= { 1.6f };
	_float					m_fCurLightY = { };

	_bool					m_bCameraChange = { false };
	_bool					m_bComputeComplete = { false };
	DIMENSION				m_ePreDimension = { DIMENSION::NONE };

	/* Shadow*/
protected:
	HRESULT		Render_Shadow();
	void		Compute_AttributeShadow();

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