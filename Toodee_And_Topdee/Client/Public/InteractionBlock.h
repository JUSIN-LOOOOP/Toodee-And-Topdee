#pragma once
#include "Block.h"

BEGIN(Client)

class CBlockState;

class CInteractionBlock abstract : public CBlock
{

protected:
	CInteractionBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteractionBlock(const CInteractionBlock& Prototype);
	virtual ~CInteractionBlock() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	/* State */
	HRESULT Change_State(BLOCKSTATE eNewState);
	void Request_Change_State(BLOCKSTATE eState);

	/* State Push */
	void Push(MOVEDIRECTION eMoveDir, TEXTUREDIRECTION eTextureDir, _float fPushPower);
	_bool Pushed(_float fTimeDelta);																			// 밀기

	_bool IsBlock() { return m_bBlock; }
	/* State Attach */
	void Attach(CTransform* pPlayerTransform, _float fSpeed);
	void Attached(_float fTimeDelta);	// 들기
	void Update_Attached_Position();
	_bool IsAttach() { return m_bAttach; }

	/* State Detach */
	void Detach(const _float3& vPosition, _float fSpeed);
	_bool Detached(_float fTimeDelta);									// 놓기
	
	/* State Stop */
	void FallIntoHole(_float fTimeDelta);																		// 구멍 빠지기

	/* State Push */
	_bool IsFall() { return m_bFalling; }
	_bool IsPush() { return m_eCurrentState == BLOCKSTATE::PUSH; }
	_bool IsStop() { return m_eCurrentState == BLOCKSTATE::STOP; }

protected:
#pragma region STATE PARAMETER
	/* State */
	map<BLOCKSTATE, CBlockState*>	m_States;																	// CPlayerStates 보관 맵
	CBlockState*					m_pCurrentState = { nullptr };
	BLOCKSTATE						m_eCurrentState = {};
	/* State Stop */
	_bool						m_bFallinHole = {};
	_bool						m_bFalling = {};
	_float						m_fMaxFallHeight = {};
	_float						m_fTotalFallHeight = {};
	/* State Push */
	CInteractionBlock*			m_pPushBlock = { nullptr };
	MOVEDIRECTION				m_eOwnerMoveDir = {};
	TEXTUREDIRECTION			m_eOnwerTextureDir = {};
	_float						m_fOwnerPushPower = {};
	_float3						m_vMoveTargetPosition = {};
	_float3						m_vPrevPosition = {};
	_bool						m_bMoving = {};
	_bool						m_bBlock = {};
	/* State Attach */
	CTransform*					m_pOwnerTransform = { nullptr };
	_bool						m_bAttach = {};																		// 플레이어 머리 위에 있는가
	/* State Attach + State Detach*/
	_float3						m_vTargetPosition = {};
	_float						m_fMoveSpeed = {};

	
#pragma endregion

protected:
	/* State */
	CBlockState* Find_State(BLOCKSTATE eKeyState);
	HRESULT Add_State(BLOCKSTATE eKeyState);
	void Update_State(_float fTimeDelta);
	void Clear_StateParameter();
	void Update_PushState(CInteractionBlock* pBlock);
	
	/* Collision */
	void CheckCollisionTopdeeState();												// 충돌 쳌

	_bool IsOnHole();													// Topdee 일때 밑이 구멍인지 확인?

	virtual HRESULT	Ready_State();


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END