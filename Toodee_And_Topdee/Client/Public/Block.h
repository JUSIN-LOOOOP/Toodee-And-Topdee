#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CBlockState;

class CBlock abstract : public CGameObject
{

protected:
	CBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock(const CBlock& Prototype);
	virtual ~CBlock() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

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
	_float ComputeDirDotLook(const _float3& vPosition, const _float3& vLook);

	/* State Detach */
	void Detach(const _float3& vPosition, _float fSpeed);
	_bool Detached(_float fTimeDelta);									// 놓기
	void FallIntoHole(_float fTimeDelta);																		// 구멍 빠지기

	/* State Push */
	_bool IsPush() { return m_eCurrentState == BLOCKSTATE::PUSH; }

	/* State STOP */
	_bool IsStop() { return m_eCurrentState == BLOCKSTATE::STOP; }

	
protected:
	/* Component */
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	
#pragma region STATE PARAMETER
	/* State */
	map<BLOCKSTATE, CBlockState*> m_States;																	// CPlayerStates 보관 맵
	CBlockState*		m_pCurrentState = { nullptr };
	BLOCKSTATE			m_eCurrentState = {};

	/* State Push */
	CBlock*				m_pPushBlock = { nullptr };
	MOVEDIRECTION		m_eOwnerMoveDir = {};
	TEXTUREDIRECTION	m_eOnwerTextureDir = {};
	_float				m_fOwnerPushPower = {};
	_float3				m_vMoveTargetPosition = {};
	_float3				m_vPrevPosition = {};
	_bool				m_bMoving = {};
	_bool				m_bBlock = {};
	/* State Attach */
	CTransform*			m_pOwnerTransform = { nullptr };
	_bool				m_bAttach = {};																		// 플레이어 머리 위에 있는가
	/* State Attach + State Detach*/
	_float3				m_vTargetPosition = {};
	_float				m_fMoveSpeed = {};
	
#pragma endregion

	/* Toodee Dimension*/
	_float				m_fGravityPower = {};																	// 중력
	_float				m_fMaxGravityPower = {};																// 최대 중력

protected:
	/* State */
	CBlockState* Find_State(BLOCKSTATE eKeyState);
	HRESULT Add_State(BLOCKSTATE eKeyState);
	void Update_State(_float fTimeDelta);
	void Clear_StateParameter();
	void Update_PushState(CBlock* pBlock);
	
	/* Gravity*/
	void CheckDimension();												// DIMENSION 에 따른 변경 사항 갱신할곳
	void ComputeGravity(_float fTimeDelta);								// 중력 갱신
	void Gravity();														// 중력 적용

	/* Collision */
	void CheckCollisionState();												// 충돌 쳌
	_bool IsOnHole();													// Topdee 일때 밑이 구멍인지 확인?

	
	virtual	HRESULT Ready_Components();
	virtual HRESULT	Ready_State();
	void			SetUp_BlockInfo(void* pArg);
	virtual void	SetUp_RenderState();
	virtual void	Reset_RenderState();

	_uint		m_TextureIdx = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END