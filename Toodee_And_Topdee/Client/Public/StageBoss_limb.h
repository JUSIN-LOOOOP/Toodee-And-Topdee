#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Event.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CStageBoss_limb abstract : public CGameObject
{
protected:
	CStageBoss_limb(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStageBoss_limb(const CStageBoss_limb& Prototype);
	virtual ~CStageBoss_limb() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) = 0;
	virtual void	Priority_Update(_float fTimeDelta);
	virtual void	Update(_float fTimeDelta);
	virtual void	Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT Render() = 0;
	void			Do_Attack() { m_eState = STAGEMONERSTATE::CHASE; }

protected:
	virtual HRESULT Ready_Components() = 0;
	HRESULT			Chase(_float fTimeDelta);
	HRESULT			Turn(_float fTimeDelta);
	HRESULT			HIT(_float fTimeDelta);
	HRESULT			ChangeView(_float fTimeDelta);
	HRESULT			MoveToOrigin(_float fTimeDelta);
	void			RotateToFace(_float fTimeDelta);
	void			GetSignal(const MONSTERSIGNAL& Event);
	HRESULT			Ready_SubscribeEvent(_uint iPlayerLevel);

protected:
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CComponent*		m_pTopDee = { nullptr };
	CComponent*		m_pTooDee = { nullptr };
	_uint			m_iPlayLevel = {};

	STAGEMONERSTATE	m_eState = { STAGEMONERSTATE::IDLE };
	VIEWMODE		m_eViewMode = { VIEWMODE::TOODEE };
	_float			m_fTurnTime = {};					//공격하기 전에 뱅글뱅글 도는 거
	_float			m_fIdleTurnTime = {};				//가만히 있을 때 조금씩 좌우로 움직이는 거
	_float			m_fIdleTurnDir = 1.f;				//좌우로 움직이는 거 방향 조절
	_float			m_AccAngle = 0.f;					//모드 바뀔 때
	_bool			m_eTurnFlag = { true };
	_float3			m_fInitPos = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;


};

END