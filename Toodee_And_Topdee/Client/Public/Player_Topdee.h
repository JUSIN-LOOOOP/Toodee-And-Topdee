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

	/* State ���� ȣ�� */
	virtual HRESULT Return_PrevState() override;					// Stop -> ���� ���·� ���ư���	
	virtual void Idle() override;									// Idle State Ʈ����
	virtual void Move(_uint iInputData, _float fTimeDelta) override;					// Move State Ʈ����
	virtual void Action() override;									// Action State Ʈ����
	virtual void Stop() override;									// Stop State Ʈ����
	virtual void Clear(_float3 vPortalPosition) override;									// Clear State Ʈ����



	/* State Action */
	_bool IsAttach() const { return m_bIsAttach; }					// Box ��� �ִ��� Check
	_float3 ComputeTileOutlinePosition();							// TileOutline Position Return

public:
	_bool	IsAttackSparkBlock( );


private:
	_uint			m_iPlayLevel = {};

	/* Attach Check Collider */
	CCollider*		m_pActionCheckColliderCom = { nullptr };
	CTransform*		m_pActionCheckTransformCom = { nullptr };
	CInteractionBlock*			m_pFocuseBlock = { nullptr };						// ���� ���� ���⿡ ���� ����� ��
	_float			m_fMaxDot = {};

	/* Detach */
	_float3			m_vDetachPosition = {};
	_float			m_fDotByDir[ENUM_CLASS(DETACHDIRECTION::DETACHDIRECTION_END)] = {};
	_bool			m_bCanDetach = {};
	/* Movement */
	MOVEDIRECTION	m_eCurrentMoveDir = {};								// Texture Index ���� ���� ����
	MOVEDIRECTION	m_ePrevMoveDir = {};								// Stop ������ ������ ���� ����
	_bool			m_bIsMoving = { false };							// NextMovePosition ���ſ� Ʈ����
	_float3			m_vCurrentTileCenter = {};							// ���� Tile Center
	_float3			m_vNextMovePosition = {};							// �̵��� Tile Center

	/* State Action */
	_bool			m_bIsAttach = { false };								// Block ��� �ִ���
	CInteractionBlock*			m_pAttachBlock = { nullptr };						// ����ִ� Block
	/* Stop Movement */
	_bool			m_bIsTurnDown = { false };							// Stop ���۽� �Ʒ� ���� �����Ϸ��� Ʈ����
	_float			m_fTurnDownTime = {};								//TurnDownOnStop �����̿� Ÿ�̸� �Դϴ�.
	_float			m_fTurnDownDelay = {};

private:
	virtual void Check_Dimension() override;

	_uint KeyInput();												// Ű �Է� Return
	
	/* Movement */
	MOVEDIRECTION ComputeMoveDirection(_uint iInputData);			// �̵� ���� ���
	void Change_MoveDir(_uint iInputData);							// �̵� ���� ����
	_float3 ComputeTileCenter();										// Ÿ�� �߾� Position ���
	void MoveToTileCenter(_float fTimeDelta);						// Move to CurrentTileCenter
	
	/* Stop Movement*/
	void TurnDownOnStop(_float fTimeDelta);							// Stop State�� �������� �Ʒ� ���� �ٶ󺸱�

	/* Player Collision */
	void Check_Collision();									// �÷��̾� �浹 üũ
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