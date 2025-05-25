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

	virtual void Move(_float fTimeDelta) override;
	virtual void Action() override;

private:
	MOVEDIRECTION m_eCurrentMoveDir = {};
	/* Action ���� ��� */
	_bool		m_bIsAttach = { false };
	
	//Test
	_bool m_bInput = {};
	
private:
	_uint KeyInput();
	void Change_MoveDir(_uint iInputData);
	MOVEDIRECTION ComputeMoveDirection(_uint iInputData);

	HRESULT Ready_Components();
	HRESULT Ready_States();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();
	
public:
	static CPlayer_Topdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END