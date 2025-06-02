#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CBlockState;

class CBlock abstract : public CGameObject
{

protected:
	CBlock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock(const CBlock& Prototype);
	virtual ~CBlock() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	/* State */
	HRESULT Change_State(BLOCKSTATE eNewState);
	void Request_Change_State(BLOCKSTATE eState);

	/* State Push */
	void Push(MOVEDIRECTION eMoveDir, TEXTUREDIRECTION eTextureDir, _float fPushPower);
	_bool Pushed(_float fTimeDelta);																			// �б�
	_bool IsBlock() { return m_bBlock; }
	/* State Attach */
	void Attach(CTransform* pPlayerTransform, _float fSpeed);
	void Attached(_float fTimeDelta);	// ���
	void Update_Attached_Position();
	_bool IsAttach() { return m_bAttach; }
	_float ComputeDirDotLook(const _float3& vPosition, const _float3& vLook);

	/* State Detach */
	void Detach(const _float3& vPosition, _float fSpeed);
	_bool Detached(_float fTimeDelta);									// ����
	void FallIntoHole(_float fTimeDelta);																		// ���� ������

	/* State Push */
	_bool IsPush() { return m_eCurrentState == BLOCKSTATE::PUSH; }

	/* State STOP */
	_bool IsStop() { return m_eCurrentState == BLOCKSTATE::STOP; }

	
protected:
	/* Component */
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	
#pragma region STATE PARAMETER
	/* State */
	map<BLOCKSTATE, CBlockState*> m_States;																	// CPlayerStates ���� ��
	CBlockState*		m_pCurrentState = { nullptr };
	BLOCKSTATE			m_eCurrentState = {};

	/* State Push */
	CBlock*				m_pPushBlock = { nullptr };
	MOVEDIRECTION		m_eOwnerMoveDir = {};
	TEXTUREDIRECTION	m_eOnwerTextureDir = {};
	_float				m_fOwnerPushPower = {};
	_float3				m_vMoveTargetPosition = {};
	_float3				m_vPrevPosition = {};
	_bool				m_bMoving = {};
	_bool				m_bBlock = {};
	/* State Attach */
	CTransform*			m_pOwnerTransform = { nullptr };
	_bool				m_bAttach = {};																		// �÷��̾� �Ӹ� ���� �ִ°�
	/* State Attach + State Detach*/
	_float3				m_vTargetPosition = {};
	_float				m_fMoveSpeed = {};
	
#pragma endregion

	/* Toodee Dimension*/
	_float				m_fGravityPower = {};																	// �߷�
	_float				m_fMaxGravityPower = {};																// �ִ� �߷�

protected:
	/* State */
	CBlockState* Find_State(BLOCKSTATE eKeyState);
	HRESULT Add_State(BLOCKSTATE eKeyState);
	void Update_State(_float fTimeDelta);
	void Clear_StateParameter();
	void Update_PushState(CBlock* pBlock);
	
	/* Gravity*/
	void CheckDimension();												// DIMENSION �� ���� ���� ���� �����Ұ�
	void ComputeGravity(_float fTimeDelta);								// �߷� ����
	void Gravity();														// �߷� ����

	/* Collision */
	void CheckCollisionState();												// �浹 �n
	_bool IsOnHole();													// Topdee �϶� ���� �������� Ȯ��?

	
	virtual	HRESULT Ready_Components();
	virtual HRESULT	Ready_State();
	void			SetUp_BlockInfo(void* pArg);
	virtual void	SetUp_RenderState();
	virtual void	Reset_RenderState();

	_uint		m_TextureIdx = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END