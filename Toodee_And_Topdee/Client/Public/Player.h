#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayer abstract : public CGameObject
{
protected:
	/*	ACTION = ����, ���� ���
		STOP = �ൿ �Ұ����� �����϶� */
	enum PLAYERSTATE { PS_IDLE, PS_MOVE, PS_ACTION, PS_STOP, PS_DEAD };

	/*	���ҽ� ������ ���� */
	enum DIRECTION { DIR_L, DIR_R };

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

protected:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Begin_RenderState() PURE;
	virtual HRESULT End_RenderState() PURE;

	void Change_State();
	void Change_Dir();

protected:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureComs[PS_DEAD] = { nullptr };

	//Player ����
	PLAYERSTATE m_ePreState = {};
	PLAYERSTATE m_eCurrentState = {};

	DIRECTION m_ePreDir = {};
	DIRECTION m_eCurrentDir = {};

	//�ִϸ��̼� ����
	_uint m_iMaxAnimCount[PS_DEAD] = {};
	_uint m_iCurrentAnimCount = {};
	_float m_fAnimDelay = {};
	_float m_fAnimTime = {};

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END