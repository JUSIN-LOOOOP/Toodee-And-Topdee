#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
class CCollider;
class CShader_Water;
END

BEGIN(Client)

class CWater final : public CGameObject
{
public:
	typedef struct tagWaterDesc {
		_float3 vPosition;
		CWater** ppLink;
	}WATER_DESC;

private:
	CWater(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWater(const CWater& Prototype);
	virtual ~CWater() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void FillWater(_float fFillHeight);

private:
	CCollider* m_pColliderCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader_Water* m_pShader = { nullptr };

	DIMENSION m_eCurrentDimension = {};

	_bool	m_bFill = {};
	_float	m_fFillHeight = {};
	_float	m_fCurrentFillHeight = {};
	_float3 m_vSinkPos = {};
	_float	m_fCurrentStrength = {};
	_float	m_fTimeDelta = {};
	_float	m_fSpeed = {};

private:
	void Check_Dimension();
	void Collider_Setting();

	HRESULT Ready_Components();

	HRESULT Begin_RenderState();
	HRESULT Begin_ShaderState();

	HRESULT End_RenderState();
	HRESULT End_ShaderState();

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END