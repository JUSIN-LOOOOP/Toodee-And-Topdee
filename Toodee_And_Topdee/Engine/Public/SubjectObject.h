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


	//Observer 에게서 REPORT를 받습니다.
	//객체마다 REPORT 처리를 다르게 할 생각이라 가상함수로 뒀습니다.
	virtual void onReport(REPORT eReport) PURE;
	
	//옵저버 추가
	HRESULT Add_Observer(CObserver* pObserver);
	
	//옵저버 삭제
	void Remove_Observer(CObserver* pObserver);

protected:
	vector<CObserver*> m_Observers;

protected:
	//Observer 들에게 EVENT를 전달합니다.
	void Notify(EVENT eEvent);
	void Notify_Dead();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END