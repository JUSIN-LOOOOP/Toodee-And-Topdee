#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CCollider;
END

BEGIN(Client)

class CThirdee_Part;
class CThirdee_State;

class CPlayer_Thirdee final : public CGameObject
{
private:
	CPlayer_Thirdee(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer_Thirdee(const CPlayer_Thirdee& Prototype);
	virtual ~CPlayer_Thirdee() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	void Priority_Update(_float fTimeDelta) override;
	void Update(_float fTimeDelta) override;
	void Late_Update(_float fTimeDelta) override;
	HRESULT Render() override;

	void EnterIdle();
	void Idle(_float fTimeDelta);
	void EnterMove();
	void Move(_float fTimeDelta);
	void EnterAction();
	void Action(_float fTimeDelta);
	void Dead(_float fTimeDelta);
	void EnterClear();
	void Clear(_float fTimeDelta);
	
	HRESULT Change_State(PLAYERSTATE eNewState);
	_bool	InAction() { return m_bInAction; }

private:
	/* Component */
	CTransform*							m_pTransformCom = { nullptr };
	CThirdee_Part*						m_pRootPart = { nullptr };
	CCollider*							m_pColliderCom = { nullptr };
	CCollider*							m_pGroundCheckColliderCom = { nullptr };
	CCollider*							m_pAttachCheckColliderCom = { nullptr };

	/* Game Info */
	_uint								m_iPlayLevel = {};
	DIMENSION							m_eCurrentDimension = {};
	DIMENSION							m_ePrevDimension = {};

	/* Part */
	_float4x4							m_RootWorldMatrix = {};

	/* State */
	map<PLAYERSTATE, CThirdee_State*>	m_States;																	
	CThirdee_State*						m_pCurrentState = { nullptr };												
	PLAYERSTATE							m_eCurrentState = {};

	TEXTUREDIRECTION					m_ePushDirX = {};
	MOVEDIRECTION						m_ePushDirZ = {};


	_bool								m_bJumpStart = {};
	_bool								m_bInAction = {};

	_float								m_fTotalRotation = {};
	/* Direction */
	_float								m_fRotationRadian = {};

	/* Topdee */
	class CInteractionBlock*			m_pFocusBlock = { nullptr };
	class CInteractionBlock*			m_pAttachBlock = { nullptr };
	_bool								m_bCanDetach = {};
	_float3								m_vDetachPosition = {};
	_float3								m_vTilePoint = {};
	_float3								m_vMovePoint = {};
	_float3								m_vLookPoint = {};
	_float3								m_vLookDir = {};
	_float3								m_vDirPoint = {};

	/* Toodee */
	_bool								m_bFalling = {};
	_float								m_fGroundCheckColliderY = {};
	_float								m_fCurrentGravity = {};
	_float								m_fMaxGravity = {};
	_float								m_fCurrentJumpPower = {};
	_float								m_fAccumulationJumpPower = {};

private:
	HRESULT Ready_Component();
	HRESULT Ready_Bodys();
	HRESULT Ready_States();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

#pragma region 구현할거
	/* State */
	CThirdee_State* Find_State(PLAYERSTATE eKeyState);
	HRESULT Add_State(PLAYERSTATE eKeyState);

	/* All Dimension */
	_uint KeyInput();
	void Check_Dimension(_float fTimeDelta);
	void Check_Collision_PlayerPos();
	void Check_Collision_Enemy(CGameObject* pGameObject);
	void Check_Collision_Portal(CGameObject* pGameObject);
	/* Topdee Dimension */
	void Check_Topdee_State(_uint iInputData, _float fTimeDelta);
	void Check_Topdee_Collision();
	void Check_Collision_InteractionBlock(CGameObject* pGameObject);
	void Check_Collision_Attach();
	void Check_Collision_Detach();
	_float3 Compute_Detach_Position(DETACHDIRECTION eDir);
	void Interaction();
	void Topdee_Check_Center();
	void Compute_PushDirection(_float3 vMovePos);

	void Topdee_Direction(_uint iInputData, _float fTimeDelta);
	void Topdee_Move(_float fTimeDelta);
	void Topdee_Action(_float fTimeDelta);

	/* Toodee Dimension */
	void Check_Toodee_State(_uint iInputData, _float fTimeDelta);
	void Check_Toodee_Collision();
	void Check_Collision_ToodeState();
	void Check_Collision_Ground();
	void Toodee_Direction(_uint iInputData, _float fTimeDelta);
	void Toodee_Move(_float fTimeDelta);
	void Toodee_Action(_float _fTimeDelta);

	void Gravity(_float fTimeDelta);
#pragma endregion

public:
	static CPlayer_Thirdee* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END