#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPoolableObject abstract : public CGameObject
{
protected:
	CPoolableObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPoolableObject(const CPoolableObject& Prototype);
	virtual ~CPoolableObject() = default;

public:	
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void Priority_Update(_float fTimeDelta) PURE;
	virtual void Update(_float fTimeDelta) PURE;
	virtual void Late_Update(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public:
	virtual HRESULT Initialize_Pool(void* pArg) PURE;

public:
	void Set_Active(_bool bActive) { m_bActive = bActive; }

protected:
	_bool	m_bActive = { false };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END