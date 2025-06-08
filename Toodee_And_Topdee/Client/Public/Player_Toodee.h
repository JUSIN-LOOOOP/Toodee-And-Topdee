#pragma once

#include "Player.h"

BEGIN(Client)

class CPlayer_Toodee final : public CPlayer
{


private:
	CPlayer_Toodee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_Toodee(const CPlayer_Toodee& Prototype);
	virtual ~CPlayer_Toodee() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Return_PrevState() override;						
	virtual void Idle() override;									
	virtual void Move(_uint iInputData, _float fTimeDelta) override;
	void Swim(_uint iInputData, _float fTimeDelta);
	virtual void Action() override;									
	virtual void Stop() override;									
	virtual void Clear(_float3 vPortalPosition) override;
	virtual void Dead() override;

	_bool InWater() { return m_bInWater; }
	void OutWater() { m_bOutWater = true; }

private:								
	_uint		m_iPlayLevel = {};
	/* Ground Check Collider */										   
	CCollider*  m_pGroundCheckColliderCom = { nullptr };			// 지면 체크용 Collider
	CTransform* m_pGroundCheckTransformCom = { nullptr };			// 지면 체크용 Collider에게 붙일 Transform
	_float		m_fGroundCheckPosZ = {};							// 지면 체크용 COllider를 플레이어 발 밑에 두기 위해 + 해줄 PosZ 값
																	   
	/* Jump */														   
	JUMPSTATE	m_eJumpState = {};									// 점프 상태
	_float		m_fStartJumpPower = {};
	_float		m_fCurrentJumpPower = {};							// 점프 높이를 조절 POWER
	_float		m_fAccumulationJumpPower = {};						// 현재 증가된 점프량
	_float		m_fIncreaseJumpPower = {};							// 한프레임당 증가할수 있는 점프파워
	_float		m_fMaxIncreaseJumpPower = {};						// 점프 증가량 최대치
	_float		m_fGravityPower = {};								// 중력
	_bool		m_bOnThePortal = {};
	_bool		m_bOnTheStorm = { false };
	_bool		m_bInWater = { false };
	_bool		m_bOutWater = { false };
private:															   
	_uint KeyInput();												//  키 입력 Return
																	   
	void Action_Jump(_float fTimeDelta);							// CurrentState == Action State 일때 실행
	void Gravity(_float fTimeDelta);								// 중력 적용
	void Compute_Gravity(_float fTimeDelta);						// 중력 갱신
																	   
	void Check_Collision();											// 플레이어 충돌 체크
	void Check_Collision_PlayerState();
	void Check_Collision_BlockBreak(CGameObject* pGameObject);
	void Check_Collision_Dead(CGameObject* pGameObject);
	void Check_Collision_Portal(CGameObject* pGameObject);
	void Check_Collision_Key(CGameObject* pGameObject);
	void Check_Collision_Storm(CGameObject* pGameObject);
	void Check_Collision_Water(CGameObject* pGameObject);
	void Check_Grounded();											// 지면 체크
	void Check_OnGround(CGameObject* pGameObject);
;	HRESULT Ready_Components();
	HRESULT Ready_SubscribeEvent(_uint iPlayerLevel);
	HRESULT Ready_States();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CPlayer_Toodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END