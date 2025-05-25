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

	virtual void Move(_float fTimeDelta) override;
	virtual void Action() override;

private:
	MOVEDIRECTION m_eCurrentMoveDir = {};
	/* Action 상자 들기 */
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