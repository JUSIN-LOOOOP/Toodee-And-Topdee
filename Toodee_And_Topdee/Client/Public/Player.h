#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayerState;

class CPlayer abstract : public CGameObject
{

protected:
	/*	ACTION = 점프, 상자 들기
		STOP = 행동 불가능한 시점일때 */

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
	//State
	HRESULT Change_State(PLAYERSTATE eNewState);
	HRESULT Return_PrevState();
	virtual void Move(_float fTimeDelta) PURE;
	virtual void Action() PURE;
	virtual void Stop() PURE;
	void Clear();

	_bool CanMoveInAction() const { return m_bMoveInAction; }
	_bool InAction() const { return m_bInAction; }
	_bool CanClear() const { return m_bCanClear; }
	
protected:
	void Change_TextureDir(TEXTUREDIRECTION eTextureDirection);
	CPlayerState* Find_State(PLAYERSTATE eKeyState);
	HRESULT Add_State(PLAYERSTATE eKeyState, void* pArg);
	//TextureDirection
	void ComputeTextureDirection(_uint iInputData);
	_uint ComputeStopAnimCount(PLAYERSTATE eCurrentState);

protected:
	CTransform*						m_pTransformCom = { nullptr };
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureComs[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)] = { nullptr };
	
	//Player 상태
	PLAYERSTATE_DESC				m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)];
	//CPlayerStates 보관 맵
	map<PLAYERSTATE, CPlayerState*> m_States;
	//현재 CPlayerState 포인터
	CPlayerState*					m_pCurrentState = { nullptr };
	//현재 State enum class
	PLAYERSTATE						m_eCurrentState = {};

	//Stop State용
	CPlayerState*					m_pPrevState = { nullptr };
	PLAYERSTATE						m_ePrevState = {};
	_uint							m_iStopAnimCount = {};


	//액션 중 움직임 가능여부
	_bool							m_bMoveInAction = {};
	//액션 중 판별
	_bool							m_bInAction = {};
	//텍스쳐 방향
	TEXTUREDIRECTION				m_eCurrentTextureDir = {};
	//현재 애니메이션 번호
	_uint							m_iCurrentAnimCount = {};
	//클리어
	_bool							m_bCanClear = {};
	_bool							m_bMoveToPotal = {};
	_bool							m_bClearAnimStart = {};
	//스테이지 세팅하며 세팅
	_float3							m_vPotalPosition = {};
	

	////////////////////////////////////
	//Clear Animation 시작 위치까지 동시에 도착하기 위한 스피드
	_float m_fClearSpeedPerSec = {};

	//Spiral에 넘겨주기 위한 포탈과의 거리
	_float3 m_vPotalStartPosition = {};
	_float m_fPotalDistance = {};
	_bool m_bClear = {};

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END