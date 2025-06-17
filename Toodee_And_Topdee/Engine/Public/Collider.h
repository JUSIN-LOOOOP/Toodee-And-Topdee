#pragma once

#include "Component.h"
#include <array>
/* 원형 컴포넌트 만들 때 아래와 같이 나누기*/
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
	
		_float3		vColliderScale = {};	/* 콜라이더 크기*/
		_float3		vColliderPosion = {};	/* 콜라이더 고정 위치*/
		_bool		bIsFixed = { false };	/* 위치 고정 여부 */

	}COLLIDER_DESC;

private:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT			Initialize_Prototype(COLLIDER_SHAPE eType);
	virtual HRESULT			Initialize(void* pArg) override;

	/* 사용자가 쓰는 함수들 */
public:
	/* 충돌 진입 시점 */
	const _bool				OnCollisionEnter() const { return m_eState == COLLIDER_STATE::ENTRY; }
	const _bool				OnCollisionStay()  const { return m_eState == COLLIDER_STATE::STAY; }
	const _bool				OnCollisionExit()  const { return m_eState == COLLIDER_STATE::EXIT; }

	/* 충돌 on/off */
	const _bool				Collision_IsActive()  const { return m_bisTrigger; }
	void					Collision_Off() { Remove_Others(); m_bisTrigger = false; }
	void					Collision_On() { Remove_Others(); m_bisTrigger = true; }

	/* 콜라이더 상태 변경 */
	void					ApplyFixedPosition(_float3 vPosition) { m_vPosition = vPosition; m_bIsFixed = true; }
	void					SyncWithOwner() { m_bIsFixed = false; }
	void					Set_Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);

	/* 충돌중인 객체 찾기 (마지막 충돌체 or 모든 충돌체) */
	_bool					GetOverlapAll(vector<class CGameObject*>*& pList);
	class CGameObject*		GetOverlapTarget();

	/* 단일 or 여러 충돌체와의 침투거리등등 (GetCollisionsOffset은 현재 m_strCompares하고만 충돌중  */
	const COLLIDER_DIR		DetectCollisionDirection(_float* distance = nullptr) const;
	const _bool				GetCollisionsOffset(_float3* distance,const _wstring strComponentTag = TEXT("Com_Collider")) const;
	
	/*  가장 가까운 충돌체의 정보 꺼내오기 */
	TARGET_RESULT			FindNearestTarget(const _wstring strName = TEXT(""));

	/* 렌더 방법 */
	HRESULT					Render();
	HRESULT					OBB_Render();

	/* 현재 콜라이더의 정보 */
	const _float3			Get_ColliderScaled() { return m_vScale; };
	const _float3			Get_ColliderPosition(); 
	class CTransform*		Get_Transform() { return m_pTransform; };
	class CGameObject*		Get_GameObject() { return	m_pOwner; }

	/* Engine 함수 */
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
	_bool						m_bisTrigger = { true }; //충돌 영역 활성/ 비활성

	class CTransform* m_pTransform = { nullptr };	//이 컴포넌트를 들고 있는 객체의 트랜스폼
	class CGameObject* m_pOwner = { nullptr };		//순환 참조되므로 카운터를 올리지 않는다.

	vector<class CGameObject*>	m_pOthers;		/* 충돌 오브젝트 모음 */


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
