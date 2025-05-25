#pragma once

#include "Component.h"

/* 원형 컴포넌트 만들 때 아래와 같이 나누기*/
/* Prototype_Component_Collider_Rect*/
/* Prototype_Component_Collider_Cube*/


BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum class COLLIDERSTATE { NONE, ENTRY, STAY, EXIT };

	typedef struct tagColliderDesc
	{
		class CTransform* pTransform;
		class CGameObject* pOwner;

	}COLLIDER_DESC;

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT		Initialize_Prototype(COLLIDER_SHAPE eType);
	virtual HRESULT		Initialize(void* pArg) override;

	/* 사용자가 쓰는 함수들 */
public:
	const _bool			OnCollisionEnter() { return m_eState == COLLIDERSTATE::ENTRY; }
	const _bool			OnCollisionStay() { return m_eState == COLLIDERSTATE::STAY; }
	const _bool			OnCollisionExit() { return m_eState == COLLIDERSTATE::EXIT; }

	const _bool			Collider_IsActive() { return m_bisTrigger; }
	void				Collision_Off() { m_bisTrigger = false; }
	void				Collision_On() { m_bisTrigger = true; }

	HRESULT				GetOverlapAll(list<class CGameObject*>& pList)
	{
		if (m_eState == COLLIDERSTATE::NONE) return E_FAIL;
		pList = m_pOthers;
		return S_OK;
	}
	/* 정확할지 모르겠음... 충돌 가장 마지막에 들어 온 객체를 꺼내오는거임.
	만약 정확하지 않으면 GetOverlapAll에서 꺼내서 이름 비교하며 처리하기*/
	class CGameObject*	GetOverlapTarget()
	{
		return (m_eState == COLLIDERSTATE::NONE) ?  nullptr : m_pOthers.back();
	}
	
public:
	COLLIDER_SHAPE		Reference_Collider_Info(class CTransform** pTransform, class CGameObject** pGameObject);
	void				Set_State(COLLIDERSTATE eState) { m_eState = eState; }
	void				Add_Other(class CGameObject* pGameObject);
	void				Remove_Other(class CGameObject* pGameObject);

private:
	 COLLIDER_SHAPE				m_eShape = {};
	 COLLIDERSTATE				m_eState = { COLLIDERSTATE::NONE };
	 _bool						m_bisTrigger = {}; //충돌 영역 활성/ 비활성

	 /* 매번 CGameObject를 통해 find로 찾아서 트랜스폼을 찾는 것보다 들고 있는게 낫다. */
	 class CTransform*			m_pTransform = { nullptr };//이 컴포넌트를 들고 있는 객체의 트랜스폼
	 class CGameObject*			m_pOwner = { nullptr }; //이 컴포넌트를 들고 있는 객체

	 list<class CGameObject*>	m_pOthers; // 충돌중인 객체 모음집.


public:
	static	CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER_SHAPE eType);
	virtual CCollider* Clone(void* pArg) override;
	virtual void Free() override;

};

END
