#pragma once
#include "Player.h"

BEGIN(Client)

class CInteractionBlock;

class CPlayer_Topdee final : public CPlayer
{
private:
	CPlayer_Topdee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_Topdee(const CPlayer_Topdee& Prototype);
	virtual ~CPlayer_Topdee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	/* State 에서 호출 */
	virtual HRESULT Return_PrevState() override;					// Stop -> 이전 상태로 돌아가기	
	virtual void Idle() override;									// Idle State 트리거
	virtual void Move(_uint iInputData, _float fTimeDelta) override;					// Move State 트리거
	virtual void Action() override;									// Action State 트리거
	virtual void Stop() override;									// Stop State 트리거
	virtual void Clear(_float3 vPortalPosition) override;									// Clear State 트리거



	/* State Action */
	_bool IsAttach() const { return m_bIsAttach; }					// Box 들고 있는지 Check
	_float3 ComputeTileOutlinePosition();							// TileOutline Position Return

public:
	_bool	IsAttackSparkBlock( );


private:
	_uint			m_iPlayLevel = {};

	/* Attach Check Collider */
	CCollider*		m_pActionCheckColliderCom = { nullptr };
	CTransform*		m_pActionCheckTransformCom = { nullptr };
	CInteractionBlock*			m_pFocuseBlock = { nullptr };						// 현재 보는 방향에 가장 가까운 블럭
	_float			m_fMaxDot = {};

	/* Detach */
	_float3			m_vDetachPosition = {};
	_float			m_fDotByDir[ENUM_CLASS(DETACHDIRECTION::DETACHDIRECTION_END)] = {};
	_bool			m_bCanDetach = {};
	/* Movement */
	MOVEDIRECTION	m_eCurrentMoveDir = {};								// Texture Index 계산용 방향 보관
	MOVEDIRECTION	m_ePrevMoveDir = {};								// Stop 해제시 돌려줄 방향 보관
	_bool			m_bIsMoving = { false };							// NextMovePosition 갱신용 트리거
	_float3			m_vCurrentTileCenter = {};							// 현재 Tile Center
	_float3			m_vNextMovePosition = {};							// 이동할 Tile Center

	/* State Action */
	_bool			m_bIsAttach = { false };								// Block 들고 있는지
	CInteractionBlock*			m_pAttachBlock = { nullptr };						// 들고있는 Block
	/* Stop Movement */
	_bool			m_bIsTurnDown = { false };							// Stop 시작시 아래 방향 보게하려는 트리거
	_float			m_fTurnDownTime = {};								//TurnDownOnStop 딜레이용 타이머 입니다.
	_float			m_fTurnDownDelay = {};

private:
	virtual void Check_Dimension() override;

	_uint KeyInput();												// 키 입력 Return
	
	/* Movement */
	MOVEDIRECTION ComputeMoveDirection(_uint iInputData);			// 이동 방향 계산
	void Change_MoveDir(_uint iInputData);							// 이동 방향 갱신
	_float3 ComputeTileCenter();										// 타일 중앙 Position 계산
	void MoveToTileCenter(_float fTimeDelta);						// Move to CurrentTileCenter
	
	/* Stop Movement*/
	void TurnDownOnStop(_float fTimeDelta);							// Stop State로 변했을때 아래 방향 바라보기

	/* Player Collision */
	void Check_Collision();									// 플레이어 충돌 체크
	void Check_Collision_PlayerState();
	void Check_Collision_Dead(CGameObject* pGameObject);
	void Check_Collision_InteractionBlock(CGameObject* pGameObject);
	void Check_Collision_Portal(CGameObject* pGameObject);
	void Check_Collision_Key(CGameObject* pGameObject);

	/* Attach  */
	void Check_AttachCollisionState();
	void Interaction();
	_float3 Compute_Look();

	/* Detach */
	void Check_DetachCollisionState();
	_float3 Compute_Detach_Position(DETACHDIRECTION eDir);

	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_SubscribeEvent(_uint iPlayerLevel);
	HRESULT Ready_Outline(_uint iPlayLevel);

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	
public:
	static CPlayer_Topdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END