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
	/*	ACTION = 점프, 상자 들기
		STOP = 행동 불가능한 시점일때 */
	enum PLAYERSTATE { PS_IDLE, PS_MOVE, PS_ACTION, PS_STOP, PS_CLEAR, PS_DEAD };

	/*	리소스 반전용 방향 */
	enum DIRECTION { DIR_L, DIR_R };

	enum PLAYERTYPE { PT_TOODEE, PT_TOPDEE};

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
	_bool MoveToPotal(const _float3& vTarget, const _float3& vAxis, _float fTimeDelta);

protected:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureComs[PS_DEAD] = { nullptr };

	//Player 상태
	PLAYERSTATE m_ePreState = {};
	PLAYERSTATE m_eCurrentState = {};

	DIRECTION m_ePreDir = {};
	DIRECTION m_eCurrentDir = {};

	//애니메이션 관련
	_uint m_iMaxAnimCount[PS_DEAD] = {};
	_uint m_iCurrentAnimCount = {};
	_float m_fAnimDelay = {};
	_float m_fAnimTime = {};

	//Clear Animation 시작 위치까지 동시에 도착하기 위한 스피드
	_float m_fClearSpeedPerSec = {};

	//Spiral에 넘겨주기 위한 포탈과의 거리
	_float3 m_vPotalStartPosition = {};
	_float m_fPotalDistance = {};
	_bool m_bClear = {};

protected:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END