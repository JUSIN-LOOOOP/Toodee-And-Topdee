#pragma once
#include "Player.h"

BEGIN(Client)

class CPlayer_Topdee final : public CPlayer
{
private:
	/* 순서대로 리소스 번호 찾아감 (아래, 횡, 아래 대각선, 위 대각선, 위) */
	/* 비트 연산용 input enum class */


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


	//State 호출 함수
	virtual HRESULT Return_PrevState() override;
	virtual void Idle() override;
	virtual void Move(_float fTimeDelta) override;
	virtual void Action() override;
	virtual void Stop() override;
	virtual void Clear() override;

	//Observer 에서 받은 REPORT 처리
	virtual void onReport(REPORT eReport) override;

	_bool IsAttach() const { return m_bIsAttach; }
	_float3 ComputeTileOutlinePosition();

private:
	// Texture Index 계산용 방향 보관
	MOVEDIRECTION m_eCurrentMoveDir = {};
	// Stop 해제시 돌려줄 방향 보관
	MOVEDIRECTION m_ePrevMoveDir = {};
	// Stop 시작시 아래 방향 보게하려는 트리거
	_bool		m_bIsTurnDown = { false };
	// Action 상자 들기
	_bool		m_bIsAttach = { true };
	//TurnDownOnStop 딜레이용 타이머 입니다.
	_float		m_fTurnDownTime = {};
	_float		m_fTurnDownDelay = {};

	//Idle 타일단위
	_bool		m_bIsMoving = { false };
	_float3		m_vCurrentTileCenter = {};
	_float3		m_vNextMovePosition = {};
private:
	_uint KeyInput();
	void Change_MoveDir(_uint iInputData);
	MOVEDIRECTION ComputeMoveDirection(_uint iInputData);

	void ComputeTileCenter();
	void MoveToTileCenter(_float fTimeDelta);

	void TurnDownOnStop(_float fTimeDelta);
	

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