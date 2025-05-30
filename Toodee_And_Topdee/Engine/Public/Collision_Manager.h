#pragma once
#include "Base.h"

/* 콜라이더 사본들을 저장함, */

/* 레벨 별 사본들 가지고 충돌 처리를 한다. */

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
	void			Delete_Collider(_uint iLevelIndex, class CCollider** ppCollider); /* 1개의 오브젝트 삭제 */
	void			Clear(_uint iLevelIndex);	/* 현재 레벨의 오브젝트 전부 삭제 */
	void			Check_Collision(_uint iLevelIndex, class CCollider* pCollider);

private:
	_uint										m_iNumLevels = {};
	map<class CCollider*, COLLISION_INFO>*		m_pColliders = { nullptr };

private:
	void			Check_Deleted();	/* nullptr가 아니면 삭제  */

public:
	static CCollision_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

END