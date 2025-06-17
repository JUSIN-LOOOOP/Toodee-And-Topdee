#pragma once

#include "Component.h"
#include <array>
/* ���� ������Ʈ ���� �� �Ʒ��� ���� ������*/
/* Prototype_Component_Collider_Rect*/
/* Prototype_Component_Collider_Cube*/

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum class COLLIDER_STATE { NONE, ENTRY, STAY, EXIT };
	typedef struct tagTargetResult
	{
		class CGameObject*	pGameObject = { nullptr };
		_float				fDist = { 0.f };
		_float3				vDirection = { 0.f,0.f,0.f };
		_float3				vTargetPosition = { 0.f,0.f,0.f };
	}TARGET_RESULT;

	typedef struct tagColliderDesc
	{	
		class CGameObject* pOwner;
		class CTransform* pTransform;
	
		_float3		vColliderScale = {};	/* �ݶ��̴� ũ��*/
		_float3		vColliderPosion = {};	/* �ݶ��̴� ���� ��ġ*/
		_bool		bIsFixed = { false };	/* ��ġ ���� ���� */

	}COLLIDER_DESC;

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT			Initialize_Prototype(COLLIDER_SHAPE eType);
	virtual HRESULT			Initialize(void* pArg) override;

	/* ����ڰ� ���� �Լ��� */
public:
	/* �浹 ���� ���� */
	const _bool				OnCollisionEnter() const { return m_eState == COLLIDER_STATE::ENTRY; }
	const _bool				OnCollisionStay()  const { return m_eState == COLLIDER_STATE::STAY; }
	const _bool				OnCollisionExit()  const { return m_eState == COLLIDER_STATE::EXIT; }

	/* �浹 on/off */
	const _bool				Collision_IsActive()  const { return m_bisTrigger; }
	void					Collision_Off() { Remove_Others(); m_bisTrigger = false; }
	void					Collision_On() { Remove_Others(); m_bisTrigger = true; }

	/* �ݶ��̴� ���� ���� */
	void					ApplyFixedPosition(_float3 vPosition) { m_vPosition = vPosition; m_bIsFixed = true; }
	void					SyncWithOwner() { m_bIsFixed = false; }
	void					Set_Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);

	/* �浹���� ��ü ã�� (������ �浹ü or ��� �浹ü) */
	_bool					GetOverlapAll(vector<class CGameObject*>*& pList);
	class CGameObject*		GetOverlapTarget();

	/* ���� or ���� �浹ü���� ħ���Ÿ���� (GetCollisionsOffset�� ���� m_strCompares�ϰ� �浹��  */
	const COLLIDER_DIR		DetectCollisionDirection(_float* distance = nullptr) const;
	const _bool				GetCollisionsOffset(_float3* distance,const _wstring strComponentTag = TEXT("Com_Collider")) const;
	
	/*  ���� ����� �浹ü�� ���� �������� */
	TARGET_RESULT			FindNearestTarget(const _wstring strName = TEXT(""));

	/* ���� ��� */
	HRESULT					Render();
	HRESULT					OBB_Render();

	/* ���� �ݶ��̴��� ���� */
	const _float3			Get_ColliderScaled() { return m_vScale; };
	const _float3			Get_ColliderPosition(); 
	class CTransform*		Get_Transform() { return m_pTransform; };
	class CGameObject*		Get_GameObject() { return	m_pOwner; }

	/* Engine �Լ� */
public:
	COLLIDER_SHAPE			Reference_Collider_Info(COLLIDER_DESC& desc);
	void					Add_Others(const vector<CGameObject*>& currentCollisions);
	void					Remove_Others();
	void					Set_State(COLLIDER_STATE eState) { m_eState = eState; }

private:
	array<_wstring, 4>			m_strCompares = { TEXT("Interaction"), TEXT("Wall"), TEXT("Block"),TEXT("Hole") };

	_float3						m_vScale = { };
	_float3						m_vPosition = { };
	_bool						m_bIsFixed = { };

	COLLIDER_SHAPE				m_eShape = { };
	COLLIDER_STATE				m_eState = { COLLIDER_STATE::NONE };
	_bool						m_bisTrigger = { true }; //�浹 ���� Ȱ��/ ��Ȱ��

	class CTransform* m_pTransform = { nullptr };	//�� ������Ʈ�� ��� �ִ� ��ü�� Ʈ������
	class CGameObject* m_pOwner = { nullptr };		//��ȯ �����ǹǷ� ī���͸� �ø��� �ʴ´�.

	vector<class CGameObject*>	m_pOthers;		/* �浹 ������Ʈ ���� */


	/* Render */
private:
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9				m_pIB = { nullptr };

	_uint								m_iNumVertices = { };
	_uint								m_iVertexStride = { }; 
		_uint								m_iFVF = {};
	D3DPRIMITIVETYPE					m_ePrimitiveType = {};
	_uint								m_iNumPrimitive = {};

	_uint								m_iNumIndices = {};
	_uint								m_iIndexStride = { };
	D3DFORMAT							m_eIndexFormat = {};

	_float3								m_vPoint[8];

public:
	static	CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER_SHAPE eType);
	virtual CCollider* Clone(void* pArg) override;
	virtual void Free() override;

};

END
