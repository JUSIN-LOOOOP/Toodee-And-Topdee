#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CObserver;

class ENGINE_DLL CSubjectObject abstract : public CGameObject
{
protected:
	CSubjectObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSubjectObject(const CSubjectObject& Prototype);
	virtual ~CSubjectObject() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void Priority_Update(_float fTimeDelta) PURE;
	virtual void Update(_float fTimeDelta) PURE;
	virtual void Late_Update(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

	void Add_Observer(CObserver* pObserver);

protected:
	void Notify(CObserver::EVENT eEvent);

protected:
	vector<CObserver*> m_Observers;


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END