#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CCollider;
class CTexture;
class CVIBuffer_DiffuseCube;

END


BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	typedef struct tagMonsterDesc
	{
		CTransform* TargetTransformCom{};

	}MONSTER_DESC;

protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


protected:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTargetTransformCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	/* Shadow*/
protected:
	CVIBuffer_DiffuseCube* m_VIBufferCom_Diffuse = { nullptr };
	_float4x4				m_matrixShadow = {};

	/* Shadow : switching - camera angle */
	_float					m_fToodeeAngle = { 390.f };
	_float					m_fTopdeeAngle = { 210.f };
	_float					m_fCurAngle = { };

	_float					m_fToodeeLightDirY = { 1.8f };
	_float					m_fTopdeeLightDirY = { 1.6f };
	_float					m_fCurLightY = { };

	_bool					m_bCameraChange = { false };
	_bool					m_bComputeComplete = { false };
	DIMENSION				m_ePreDimension = { DIMENSION::NONE };

	/* Shadow*/
protected:
	virtual HRESULT		Render_Shadow() = 0;
	virtual void		Compute_AttributeShadow() = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END