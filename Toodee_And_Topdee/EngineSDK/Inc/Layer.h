#pragma once
#include "Base.h"

/* CObject_Manager���� ������ �纻 ���ӿ�����Ʈ�� �����ϴ� Ŭ���� */

/*	�ֿ���
		�����Ǿ� �ִ� �纻 ���ӿ�����Ʈ���� Priority_Update, Update, Late_Update�� ���
		�纻 ���ӿ�����Ʈ ����
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