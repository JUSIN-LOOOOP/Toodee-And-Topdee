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
	virtual void Move(_float fTimeDelta) override;					
	virtual void Action() override;									
	virtual void Stop() override;									
	virtual void Clear() override;									
	
	virtual void onReport(REPORT eReport, CSubjectObject* pSubject) override;
private:															   								   
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
																	   
private:															   
	_uint KeyInput();												//  Ű �Է� Return
																	   
	void Action_Jump(_float fTimeDelta);							// CurrentState == Action State �϶� ����
	void Gravity(_float fTimeDelta);								// �߷� ����
	void Compute_Gravity(_float fTimeDelta);						// �߷� ����
																	   
	void Check_CollisionState();									// �÷��̾� �浹 üũ
	void Check_Grounded();											// ���� üũ

;	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Ready_Observers();

	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CPlayer_Toodee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END