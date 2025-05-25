#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayerState;

class CPlayer abstract : public CGameObject
{

protected:
	/*	ACTION = ����, ���� ���
		STOP = �ൿ �Ұ����� �����϶� */

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
	//State
	HRESULT Change_State(PLAYERSTATE eNewState);
	HRESULT Return_PrevState();
	virtual void Move(_float fTimeDelta) PURE;
	virtual void Action() PURE;
	virtual void Stop() PURE;
	void Clear();

	_bool CanMoveInAction() const { return m_bMoveInAction; }
	_bool InAction() const { return m_bInAction; }
	_bool CanClear() const { return m_bCanClear; }
	
protected:
	void Change_TextureDir(TEXTUREDIRECTION eTextureDirection);
	CPlayerState* Find_State(PLAYERSTATE eKeyState);
	HRESULT Add_State(PLAYERSTATE eKeyState, void* pArg);
	//TextureDirection
	void ComputeTextureDirection(_uint iInputData);
	_uint ComputeStopAnimCount(PLAYERSTATE eCurrentState);

protected:
	CTransform*						m_pTransformCom = { nullptr };
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureComs[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)] = { nullptr };
	
	//Player ����
	PLAYERSTATE_DESC				m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END)];
	//CPlayerStates ���� ��
	map<PLAYERSTATE, CPlayerState*> m_States;
	//���� CPlayerState ������
	CPlayerState*					m_pCurrentState = { nullptr };
	//���� State enum class
	PLAYERSTATE						m_eCurrentState = {};

	//Stop State��
	CPlayerState*					m_pPrevState = { nullptr };
	PLAYERSTATE						m_ePrevState = {};
	_uint							m_iStopAnimCount = {};


	//�׼� �� ������ ���ɿ���
	_bool							m_bMoveInAction = {};
	//�׼� �� �Ǻ�
	_bool							m_bInAction = {};
	//�ؽ��� ����
	TEXTUREDIRECTION				m_eCurrentTextureDir = {};
	//���� �ִϸ��̼� ��ȣ
	_uint							m_iCurrentAnimCount = {};
	//Ŭ����
	_bool							m_bCanClear = {};
	_bool							m_bMoveToPotal = {};
	_bool							m_bClearAnimStart = {};
	//�������� �����ϸ� ����
	_float3							m_vPotalPosition = {};
	

	////////////////////////////////////
	//Clear Animation ���� ��ġ���� ���ÿ� �����ϱ� ���� ���ǵ�
	_float m_fClearSpeedPerSec = {};

	//Spiral�� �Ѱ��ֱ� ���� ��Ż���� �Ÿ�
	_float3 m_vPotalStartPosition = {};
	_float m_fPotalDistance = {};
	_bool m_bClear = {};

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END