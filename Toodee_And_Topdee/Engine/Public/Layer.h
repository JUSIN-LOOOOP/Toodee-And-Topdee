#pragma once
#include "Base.h"

/* CObject_Manager에서 생성된 사본 게임오브젝트를 보관하는 클래스 */

/*	주요기능
		보관되어 있는 사본 게임오브젝트들의 Priority_Update, Update, Late_Update를 담당
		사본 게임오브젝트 보관
*/

BEGIN(Engine)

class CLayer final :  public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	HRESULT Add_GameObject(class CGameObject* pGameObject) {
		m_GameObjects.push_back(pGameObject);
		return S_OK;
	}


private:
	//list<class CGameObject*> m_GameObjects;

	map<const _wstring, class CGameObject*> m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END