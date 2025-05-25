#pragma once
#include "Base.h"

/* 콜라이더 사본들을 저장함, */

/* 레벨 별 사본들 가지고 충돌 처리를 한다. */

BEGIN(Engine)

class CCollision_Manager final :  public CBase
{
	struct COLLIDER_STATE_INFO
	{
		class CCollider*	pCollider;
		_bool				bPreState;
		_bool				bCurState;
		
	};

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT			Initialize(_uint iNumLevels);
	void			Update(_float fTimeDelta);
	
public:
	HRESULT			Add_Collider(_uint iLevelIndex, COLLIDER_SHAPE etype, class CCollider** pCollider);
	void			Clear(_uint iLevelIndex);

private:
	_uint										m_iNumLevels = {};
	multimap<COLLIDER_SHAPE, COLLIDER_STATE_INFO>*	m_pColliders = { nullptr };

private:
	void			Check_Deleted();
	void			Check_Collision();

public:
	static CCollision_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

END