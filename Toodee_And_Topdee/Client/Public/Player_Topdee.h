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



	/* State 에서 호출 */
	virtual HRESULT Return_PrevState() override;					// Stop -> 이전 상태로 돌아가기	
	virtual void Idle() override;									// Idle State 트리거
	virtual void Move(_float fTimeDelta) override;					// Move State 트리거
	virtual void Action() override;									// Action State 트리거
	virtual void Stop() override;									// Stop State 트리거
	virtual void Clear() override;									// Clear State 트리거

	/* Observer */
	virtual void onReport(REPORT eReport) override;					// Observer가 주는 REPORT 처리

	_bool IsAttach() const { return m_bIsAttach; }					// Box 들고 있는지 Check
	_float3 ComputeTileOutlinePosition();							// TileOutline Position Return

private:

	MOVEDIRECTION m_eCurrentMoveDir = {};							// Texture Index 계산용 방향 보관

	MOVEDIRECTION m_ePrevMoveDir = {};								// Stop 해제시 돌려줄 방향 보관

	_bool		m_bIsTurnDown = { false };							// Stop 시작시 아래 방향 보게하려는 트리거

	_bool		m_bIsAttach = { true };								// Box 들고 있는지

	_float		m_fTurnDownTime = {};								//TurnDownOnStop 딜레이용 타이머 입니다.
	_float		m_fTurnDownDelay = {};

	/* Movement */
	_bool		m_bIsMoving = { false };							// NextMovePosition 갱신용 트리거
	_float3		m_vCurrentTileCenter = {};							// 현재 Tile Center
	_float3		m_vNextMovePosition = {};							// 이동할 Tile Center

private:
	_uint KeyInput();												// 키 입력 Return
	
	MOVEDIRECTION ComputeMoveDirection(_uint iInputData);			// 이동 방향 계산
	void Change_MoveDir(_uint iInputData);							// 이동 방향 갱신
	
	void ComputeTileCenter();										// 타일 중앙 Position 계산
	
	void MoveToTileCenter(_float fTimeDelta);						// Move to CurrentTileCenter
	
	void TurnDownOnStop(_float fTimeDelta);							// Stop State로 변했을때 아래 방향 바라보기

	void Check_CollisionState();									// 플레이어 충돌 체크

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