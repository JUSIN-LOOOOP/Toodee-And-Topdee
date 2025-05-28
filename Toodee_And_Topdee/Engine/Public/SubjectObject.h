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


	//Observer ���Լ� REPORT�� �޽��ϴ�.
	//��ü���� REPORT ó���� �ٸ��� �� �����̶� �����Լ��� �׽��ϴ�.
	virtual void onReport(REPORT eReport) PURE;
	
	//������ �߰�
	HRESULT Add_Observer(CObserver* pObserver);
	
	//������ ����
	void Remove_Observer(CObserver* pObserver);

protected:
	vector<CObserver*> m_Observers;

protected:
	//Observer �鿡�� EVENT�� �����մϴ�.
	void Notify(EVENT eEvent);
	void Notify_Dead();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END