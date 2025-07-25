#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)

class CFinger : public CGameObject
{
	enum class FIREBALLSTATE { CREATE, CHASE };
private:
	CFinger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFinger(const CFinger& Prototype);
	virtual ~CFinger() = default;
public:
	virtual			HRESULT Initialize_Prototype() override;
	virtual			HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual			HRESULT Render() override;

private:
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	class CCollider*		m_pColliderCom = { nullptr };

	HRESULT				Ready_Components();
	HRESULT				Begin_RenderState();
	HRESULT				End_RenderState();
	_float3				m_fTargetPos = {};
	FIREBALLSTATE		m_eState = {};
	_uint				m_iTextureIdx = 0;
	_float				uFrameDelay = 0;
	_float				m_fAngle = 0;
	_bool				m_bFallFlag = false;

public:
	static CFinger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CFinger* Clone(void* pArg) override;
	virtual void			Free() override;

};

END