#pragma once

#include "Client_Defines.h"
#include "SubjectObject.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
class CObserver;
END

BEGIN(Client)

class CPlayerState;

class CPlayer abstract : public CGameObject, public CSubjectObject
{
public:
	typedef struct tagPlayerDesc {
		_float3 vPlayerStartPosition;
		_float3 vPotalPosition;
	}PLAYERDESC;

protected:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void Priority_Update(_float fTimeDelta) PURE;
	virtual void Update(_float fTimeDelta) PURE;
	virtual void Late_Update(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public:
	/* State */
	HRESULT Change_State(PLAYERSTATE eNewState);						
	virtual HRESULT Return_PrevState() PURE;							// Stop -> 이전 상태로 돌아가기
	virtual void Idle() PURE;											// Idle State 트리거
	virtual void Move(_float fTimeDelta) PURE;							// Move State 트리거
	virtual void Action() PURE;											// Action State 트리거
	virtual void Stop() PURE;											// Stop State 트리거
	virtual void Clear() PURE;											// Clear State 트리거

	/* Observer */
	virtual void onReport(REPORT eReport, CSubjectObject* pSubject) PURE;							// Observer가 주는 REPORT 처리

	_bool CanMoveInAction() const { return m_bMoveInAction; }
	_bool InAction() const { return m_bInAction; }						
	_bool CanClear() const { return m_bCanClear; }				
	_bool CanActive() const { return m_bCanActive; }			
	_bool ClearAnimStart() const { return m_bClearAnimStart; }

protected:
	void Change_TextureDir(TEXTUREDIRECTION eTextureDirection);					// TextureDirection 으로 Texture 방향 결정

	/* State Map */
	CPlayerState* Find_State(PLAYERSTATE eKeyState);							
	HRESULT Add_State(PLAYERSTATE eKeyState, void* pArg);						


	void ComputeTextureDirection(_uint iInputData);								// TextureDirection 갱신
	_uint ComputeStopAnimCount(PLAYERSTATE eCurrentState);						// Stop AnimCount 갱신

	
	void Check_Dimension();														// Toodee Topdee Check


protected:
	CTransform*						m_pTransformCom = { nullptr };
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureComs[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)] = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };

	/* State */
	PLAYERSTATE_DESC				m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)];					// Player State 보관 배열
	map<PLAYERSTATE, CPlayerState*> m_States;																	// CPlayerStates 보관 맵
	CPlayerState*					m_pCurrentState = { nullptr };												// 현재 CPlayerState 포인터
	PLAYERSTATE						m_eCurrentState = {};														// 현재 State 

	/* Stop State */
	CPlayerState*					m_pPrevState = { nullptr };													// 이전 CPlayerState 보관
	PLAYERSTATE						m_ePrevState = {};															// 이전 State 보관
	_uint							m_iStopAnimCount = {};														// Stop State Anim Index

	DIMENSION						m_eActivateDimension = {};													// 활동 가능한 Dimension
	_bool							m_bCanActive = {};															// 활동 가능한지 State 전달용 Stop State 변경 트리거

	_bool							m_bMoveInAction = {};														// 액션 중 움직임 가능여부
	_bool							m_bInAction = {};															// 액션 중 Check용
	
	TEXTUREDIRECTION				m_eCurrentTextureDir = {};													// 텍스쳐 방향
	
	_uint							m_iCurrentAnimCount = {};													//현재 애니메이션 Index

	_bool							m_bCanClear = {};															// 클리어 조건 달성 여부 ( 포탈에 2명 )
	_bool							m_bMoveToPotal = {};														// Clear 애니메이션 Start Position Move Check
	_bool							m_bClearAnimStart = {};														// Clear 애니메이션 트리거 
	_float3							m_vPotalPosition = {};														// Clear 애니메이션 Start Position
	_float m_fClearSpeedPerSec = {};																			// Clear Animation 시작 위치까지 동시에 도착하기 위한 스피드
	_float3 m_vPotalStartPosition = {};																			// Spiral에 넘겨주기 위한 포탈과의 거리
	_float m_fPotalDistance = {};																				// Clear 애니메이션 시작할 포탈과의 거리
	_bool m_bClear = {};																						// Clear Anim End

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END