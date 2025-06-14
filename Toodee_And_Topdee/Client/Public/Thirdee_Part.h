#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer;
class CTransform;
class CTexture;
END


BEGIN(Client)

class CThirdee_Part abstract : public CGameObject
{
public:
	typedef struct tagThirdeePartDesc {
		_float4x4	LocalMatrix;
		_float3		vRotationRadian;
		_float3		vLocalTransform;
		_float3		vScale;
		_wstring	strTextureTag;
		_wstring	strVIBufferTag;
		PART_TYPE	eType;
	}THIRDEEPART_DESC;


protected:
	CThirdee_Part(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	CThirdee_Part(const CThirdee_Part& Prototype);
	virtual ~CThirdee_Part() = default;

public:
	HRESULT Initialize_Part(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void AnimUpdate(_float fTimeDelta, PLAYERSTATE eState) PURE;

	void Set_ParentWorldMatrix(_float4x4 ParentWorldMatrix) {
		m_ParentWorldMatrix = ParentWorldMatrix;
	}
protected:
	vector<CThirdee_Part*> m_Childs;

	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer*	m_pVIBufferCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	
	_float4x4	m_LocalScaleMatrix = {};
	_float4x4	m_LocalRotationMatrix = {};
	_float4x4	m_LocalTransformMatrix = {};

	_float4x4	m_LocalMatrix = {};
	_float4x4	m_InitLocalMatrix = {};
	_float4x4	m_WorldMatrix = {};
	_float4x4	m_ParentWorldMatrix = {};
	PART_TYPE	m_eType = {};

	_float	m_fRadianPerSec = {};
	_float	m_fAnimTime = {};
	_float	m_fCurrentAnimRadian = {};

protected:
	HRESULT Ready_Components(_wstring& strTextureTag, _wstring& strVIBufferTag);

	HRESULT Add_Child(THIRDEEPART_DESC PartDesc);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END