#pragma once

#include "Client_Defines.h"
#include "SubjectObject.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
class CObserver;
END

BEGIN(Client)

class CPlayerState;

class CPlayer abstract : public CGameObject, public CSubjectObject
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
	/* State */
	HRESULT Change_State(PLAYERSTATE eNewState);						
	virtual HRESULT Return_PrevState() PURE;							// Stop -> ���� ���·� ���ư���
	virtual void Idle() PURE;											// Idle State Ʈ����
	virtual void Move(_float fTimeDelta) PURE;							// Move State Ʈ����
	virtual void Action() PURE;											// Action State Ʈ����
	virtual void Stop() PURE;											// Stop State Ʈ����
	virtual void Clear() PURE;											// Clear State Ʈ����

	/* Observer */
	virtual void onReport(REPORT eReport, CSubjectObject* pSubject) PURE;							// Observer�� �ִ� REPORT ó��

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

	
	void Check_Dimension();														// Toodee Topdee Check


protected:
	CTransform*						m_pTransformCom = { nullptr };
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureComs[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)] = { nullptr };
	CCollider*						m_pColliderCom = { nullptr };

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

	_bool							m_bCanClear = {};															// Ŭ���� ���� �޼� ���� ( ��Ż�� 2�� )
	_bool							m_bMoveToPotal = {};														// Clear �ִϸ��̼� Start Position Move Check
	_bool							m_bClearAnimStart = {};														// Clear �ִϸ��̼� Ʈ���� 
	_float3							m_vPotalPosition = {};														// Clear �ִϸ��̼� Start Position
	_float m_fClearSpeedPerSec = {};																			// Clear Animation ���� ��ġ���� ���ÿ� �����ϱ� ���� ���ǵ�
	_float3 m_vPotalStartPosition = {};																			// Spiral�� �Ѱ��ֱ� ���� ��Ż���� �Ÿ�
	_float m_fPotalDistance = {};																				// Clear �ִϸ��̼� ������ ��Ż���� �Ÿ�
	_bool m_bClear = {};																						// Clear Anim End

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END