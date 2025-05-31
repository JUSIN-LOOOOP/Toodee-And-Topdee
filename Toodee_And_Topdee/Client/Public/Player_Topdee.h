#pragma once
#include "Player.h"

BEGIN(Client)

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
	virtual void Move(_float fTimeDelta) override;					// Move State Ʈ����
	virtual void Action() override;									// Action State Ʈ����
	virtual void Stop() override;									// Stop State Ʈ����
	virtual void Clear() override;									// Clear State Ʈ����

	/* Observer */
	virtual void onReport(REPORT eReport) override;					// Observer�� �ִ� REPORT ó��

	_bool IsAttach() const { return m_bIsAttach; }					// Box ��� �ִ��� Check
	_float3 ComputeTileOutlinePosition();							// TileOutline Position Return

private:

	MOVEDIRECTION m_eCurrentMoveDir = {};							// Texture Index ���� ���� ����

	MOVEDIRECTION m_ePrevMoveDir = {};								// Stop ������ ������ ���� ����

	_bool		m_bIsTurnDown = { false };							// Stop ���۽� �Ʒ� ���� �����Ϸ��� Ʈ����

	_bool		m_bIsAttach = { true };								// Box ��� �ִ���

	_float		m_fTurnDownTime = {};								//TurnDownOnStop �����̿� Ÿ�̸� �Դϴ�.
	_float		m_fTurnDownDelay = {};

	/* Movement */
	_bool		m_bIsMoving = { false };							// NextMovePosition ���ſ� Ʈ����
	_float3		m_vCurrentTileCenter = {};							// ���� Tile Center
	_float3		m_vNextMovePosition = {};							// �̵��� Tile Center

private:
	_uint KeyInput();												// Ű �Է� Return
	
	MOVEDIRECTION ComputeMoveDirection(_uint iInputData);			// �̵� ���� ���
	void Change_MoveDir(_uint iInputData);							// �̵� ���� ����
	
	void ComputeTileCenter();										// Ÿ�� �߾� Position ���
	
	void MoveToTileCenter(_float fTimeDelta);						// Move to CurrentTileCenter
	
	void TurnDownOnStop(_float fTimeDelta);							// Stop State�� �������� �Ʒ� ���� �ٶ󺸱�

	void Check_CollisionState();									// �÷��̾� �浹 üũ

	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_Observers();
	HRESULT Ready_Outline();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	
public:
	static CPlayer_Topdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END