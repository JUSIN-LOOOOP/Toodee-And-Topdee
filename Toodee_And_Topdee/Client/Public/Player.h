#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"
#include "Event.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
class CVIBuffer_DiffuseCube;
class CCollider;
class CShader;
END

BEGIN(Client)

class CPlayerState;

class CPlayer abstract : public CBlendObject
{
public:
	typedef struct tagPlayerDesc {
		_float3 vPlayerStartPosition;
		_float3 vPotalPosition;
	}PLAYERDESC;

protected:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* pArg) PURE;
	virtual void Priority_Update(_float fTimeDelta) PURE;
	virtual void Update(_float fTimeDelta) PURE;
	virtual void Late_Update(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

public:
	void ClearReady(const CANCLEAREVENT& Event);

	/* State */
	HRESULT Change_State(PLAYERSTATE eNewState);						
	virtual HRESULT Return_PrevState() PURE;							// Stop -> ���� ���·� ���ư���
	virtual void Idle() PURE;											// Idle State Ʈ����
	virtual void Move(_uint iInputData, _float fTimeDelta) PURE;							// Move State Ʈ����
	virtual void Action() PURE;											// Action State Ʈ����
	virtual void Stop() PURE;											// Stop State Ʈ����
	virtual void Clear(_float3 vPortalPosition) PURE;											// Clear State Ʈ����
	virtual void Dead() PURE;

	_bool CanMoveInAction() const { return m_bMoveInAction; }
	_bool InAction() const { return m_bInAction; }						
	_bool CanClear() const { return m_bCanClear; }				
	_bool CanActive() const { return m_bCanActive; }			
	_bool ClearAnimStart() const { return m_bClearAnimStart; }

protected:
	void Change_TextureDir(TEXTUREDIRECTION eTextureDirection);					// TextureDirection ���� Texture ���� ����

	/* State Map */
	CPlayerState* Find_State(PLAYERSTATE eKeyState);							
	HRESULT Add_State(PLAYERSTATE eKeyState, void* pArg);						


	void ComputeTextureDirection(_uint iInputData);								// TextureDirection ����
	_uint ComputeStopAnimCount(PLAYERSTATE eCurrentState);						// Stop AnimCount ����

	
	virtual void Check_Dimension();														// Toodee Topdee Check
	HRESULT Begin_Shader();
	HRESULT End_Shader();

protected:
	CTransform*						m_pTransformCom = { nullptr };
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureComs[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)] = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };
	CShader*						m_pShader = { nullptr };
	CVIBuffer_DiffuseCube*			m_VIBufferCom_Diffuse = { nullptr };

	/* State */
	PLAYERSTATE_DESC				m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)];					// Player State ���� �迭
	map<PLAYERSTATE, CPlayerState*> m_States;																	// CPlayerStates ���� ��
	CPlayerState*					m_pCurrentState = { nullptr };												// ���� CPlayerState ������
	PLAYERSTATE						m_eCurrentState = {};														// ���� State 

	/* Stop State */
	CPlayerState*					m_pPrevState = { nullptr };													// ���� CPlayerState ����
	PLAYERSTATE						m_ePrevState = {};															// ���� State ����
	_uint							m_iStopAnimCount = {};														// Stop State Anim Index

	DIMENSION						m_eActivateDimension = {};													// Ȱ�� ������ Dimension
	_bool							m_bCanActive = {};															// Ȱ�� �������� State ���޿� Stop State ���� Ʈ����

	_bool							m_bMoveInAction = {};														// �׼� �� ������ ���ɿ���
	_bool							m_bInAction = {};															// �׼� �� Check��
	
	TEXTUREDIRECTION				m_eCurrentTextureDir = {};													// �ؽ��� ����
	
	_uint							m_iCurrentAnimCount = {};													//���� �ִϸ��̼� Index

	_bool							m_bEnterPortal = {};
	_bool							m_bCanClear = {};															// Ŭ���� ���� �޼� ���� ( ��Ż�� 2�� )
	_bool							m_bMoveToPotal = {};														// Clear �ִϸ��̼� Start Position Move Check
	_bool							m_bClearAnimStart = {};														// Clear �ִϸ��̼� Ʈ���� 
	
	_float3							m_vPotalPosition = {};														// Clear �ִϸ��̼� Start Position
	_float m_fClearSpeedPerSec = {};																			// Clear Animation ���� ��ġ���� ���ÿ� �����ϱ� ���� ���ǵ�
	_float3 m_vPotalStartPosition = {};																			// Spiral�� �Ѱ��ֱ� ���� ��Ż���� �Ÿ�
	_float m_fPotalDistance = {};																				// Clear �ִϸ��̼� ������ ��Ż���� �Ÿ�
	_bool m_bClear = {};																						// Clear Anim End

	/* Shadow :  switching - camera angle */
	_float					m_fToodeeAngle = { 390.f };
	_float					m_fTopdeeAngle = { 210.f };
	_float					m_fCurAngle = { };

	_float					m_fToodeeLightDirY = { 1.8f };
	_float					m_fTopdeeLightDirY = { 1.6f };
	_float					m_fCurLightY = { };

	_bool					m_bCameraChange = { false };
	_bool					m_bComputeComplete = { false };
	DIMENSION				m_ePreDimension = { DIMENSION::NONE };

	/* Shadow*/
protected:
	HRESULT		Render_Shadow();
	void		Compute_AttributeShadow();


protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END