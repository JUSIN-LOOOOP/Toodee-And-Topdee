#pragma once
#include "Player.h"

BEGIN(Client)

class CPlayer_Topdee final : public CPlayer
{
private:
	/* ������� ���ҽ� ��ȣ ã�ư� (�Ʒ�, Ⱦ, �Ʒ� �밢��, �� �밢��, ��) */
	enum MOVE_DIRECTION { MD_DOWN, MD_TRANSVERSE, MD_DIAGONAL_DOWN, MD_DIAGONAL_UP, MD_UP };
	/* ��Ʈ ����� input enum class */
	enum INPUT_DIRECTION { NONE =0, ID_UP = 1 <<0, ID_DOWN = 1 << 1, ID_LEFT = 1 << 2, ID_RIGHT = 1 << 3};

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

private:
	/*��Ʈ�������� ���� ���� */
	MOVE_DIRECTION Check_Move_Direction(_uint iInputState);

private:
	/* �÷��̾ ���� ���� */
	MOVE_DIRECTION m_eMoveDir = {};
	_uint m_iInputState = {};
	_uint m_iOldInputState = {};
	/* Action ���� ��� */
	_bool		m_bIsAttach = { false };
	
	//Test
	_bool m_bInput = {};
	
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CPlayer_Topdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END