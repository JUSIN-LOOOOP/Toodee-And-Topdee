#pragma once
#include "Base.h"

/* �ݶ��̴� �纻���� ������, */

/* ���� �� �纻�� ������ �浹 ó���� �Ѵ�. */

BEGIN(Engine)

class CCollision_Manager final :  public CBase
{
	struct COLLISION_INFO
	{
		COLLIDER_SHAPE		eType;
		_bool				bPreState;
	};

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT			Initialize(_uint iNumLevels);
	
public:
	HRESULT			Add_Collider(_uint iLevelIndex, COLLIDER_SHAPE etype, class CCollider** ppCollider); 
	void			Delete_Collider(_uint iLevelIndex, class CCollider** ppCollider); /* 1���� ������Ʈ ���� */
	void			Clear(_uint iLevelIndex);	/* ���� ������ ������Ʈ ���� ���� */
	void			Check_Collision(_uint iLevelIndex, class CCollider* pCollider);

private:
	_uint										m_iNumLevels = {};
	map<class CCollider*, COLLISION_INFO>*		m_pColliders = { nullptr };

private:
	void			Check_Deleted();	/* nullptr�� �ƴϸ� ����  */

public:
	static CCollision_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

END