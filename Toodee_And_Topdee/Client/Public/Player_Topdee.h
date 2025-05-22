#pragma once
#include "Player.h"

BEGIN(Client)

class CPlayer_Topdee final : public CPlayer
{
private:
	/* 순서대로 리소스 번호 찾아감 (아래, 횡, 아래 대각선, 위 대각선, 위) */
	enum MOVE_DIRECTION { MD_DOWN, MD_TRANSVERSE, MD_DIAGONAL_DOWN, MD_DIAGONAL_UP, MD_UP };
	/* 비트 연산용 input enum class */
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
	/*비트연산으로 방향 결정 */
	MOVE_DIRECTION Check_Move_Direction(_uint iInputState);

private:
	/* 플레이어가 보는 방향 */
	MOVE_DIRECTION m_eMoveDir = {};
	_uint m_iInputState = {};
	_uint m_iOldInputState = {};
	/* Action 상자 들기 */
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