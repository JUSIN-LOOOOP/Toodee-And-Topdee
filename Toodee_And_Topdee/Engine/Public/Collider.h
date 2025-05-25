#pragma once

#include "Component.h"

/* ���� ������Ʈ ���� �� �Ʒ��� ���� ������*/
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

	/* ����ڰ� ���� �Լ��� */
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
	/* ��Ȯ���� �𸣰���... �浹 ���� �������� ��� �� ��ü�� �������°���.
	���� ��Ȯ���� ������ GetOverlapAll���� ������ �̸� ���ϸ� ó���ϱ�*/
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
	 _bool						m_bisTrigger = {}; //�浹 ���� Ȱ��/ ��Ȱ��

	 /* �Ź� CGameObject�� ���� find�� ã�Ƽ� Ʈ�������� ã�� �ͺ��� ��� �ִ°� ����. */
	 class CTransform*			m_pTransform = { nullptr };//�� ������Ʈ�� ��� �ִ� ��ü�� Ʈ������
	 class CGameObject*			m_pOwner = { nullptr }; //�� ������Ʈ�� ��� �ִ� ��ü

	 list<class CGameObject*>	m_pOthers; // �浹���� ��ü ������.


public:
	static	CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER_SHAPE eType);
	virtual CCollider* Clone(void* pArg) override;
	virtual void Free() override;

};

END
