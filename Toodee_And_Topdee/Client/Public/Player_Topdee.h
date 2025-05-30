#pragma once
#include "Player.h"

BEGIN(Client)

class CPlayer_Topdee final : public CPlayer
{
private:
	/* ������� ���ҽ� ��ȣ ã�ư� (�Ʒ�, Ⱦ, �Ʒ� �밢��, �� �밢��, ��) */
	/* ��Ʈ ����� input enum class */


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


	//State ȣ�� �Լ�
	virtual HRESULT Return_PrevState() override;
	virtual void Idle() override;
	virtual void Move(_float fTimeDelta) override;
	virtual void Action() override;
	virtual void Stop() override;
	virtual void Clear() override;

	//Observer ���� ���� REPORT ó��
	virtual void onReport(REPORT eReport) override;

	_bool IsAttach() const { return m_bIsAttach; }
	_float3 ComputeTileOutlinePosition();

private:
	// Texture Index ���� ���� ����
	MOVEDIRECTION m_eCurrentMoveDir = {};
	// Stop ������ ������ ���� ����
	MOVEDIRECTION m_ePrevMoveDir = {};
	// Stop ���۽� �Ʒ� ���� �����Ϸ��� Ʈ����
	_bool		m_bIsTurnDown = { false };
	// Action ���� ���
	_bool		m_bIsAttach = { true };
	//TurnDownOnStop �����̿� Ÿ�̸� �Դϴ�.
	_float		m_fTurnDownTime = {};
	_float		m_fTurnDownDelay = {};

	//Idle Ÿ�ϴ���
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