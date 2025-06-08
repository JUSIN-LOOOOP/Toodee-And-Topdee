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
	CCollider*  m_pGroundCheckColliderCom = { nullptr };			// ���� üũ�� Collider
	CTransform* m_pGroundCheckTransformCom = { nullptr };			// ���� üũ�� Collider���� ���� Transform
	_float		m_fGroundCheckPosZ = {};							// ���� üũ�� COllider�� �÷��̾� �� �ؿ� �α� ���� + ���� PosZ ��
																	   
	/* Jump */														   
	JUMPSTATE	m_eJumpState = {};									// ���� ����
	_float		m_fStartJumpPower = {};
	_float		m_fCurrentJumpPower = {};							// ���� ���̸� ���� POWER
	_float		m_fAccumulationJumpPower = {};						// ���� ������ ������
	_float		m_fIncreaseJumpPower = {};							// �������Ӵ� �����Ҽ� �ִ� �����Ŀ�
	_float		m_fMaxIncreaseJumpPower = {};						// ���� ������ �ִ�ġ
	_float		m_fGravityPower = {};								// �߷�
	_bool		m_bOnThePortal = {};
	_bool		m_bOnTheStorm = { false };
	_bool		m_bInWater = { false };
	_bool		m_bOutWater = { false };
private:															   
	_uint KeyInput();												//  Ű �Է� Return
																	   
	void Action_Jump(_float fTimeDelta);							// CurrentState == Action State �϶� ����
	void Gravity(_float fTimeDelta);								// �߷� ����
	void Compute_Gravity(_float fTimeDelta);						// �߷� ����
																	   
	void Check_Collision();											// �÷��̾� �浹 üũ
	void Check_Collision_PlayerState();
	void Check_Collision_BlockBreak(CGameObject* pGameObject);
	void Check_Collision_Dead(CGameObject* pGameObject);
	void Check_Collision_Portal(CGameObject* pGameObject);
	void Check_Collision_Key(CGameObject* pGameObject);
	void Check_Collision_Storm(CGameObject* pGameObject);
	void Check_Collision_Water(CGameObject* pGameObject);
	void Check_Grounded();											// ���� üũ
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