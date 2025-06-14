#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CGiantHand final : public CGameObject
{
private:
	CGiantHand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGiantHand(const CGiantHand& Prototype);
	virtual ~CGiantHand() = default;

public:
	virtual			HRESULT Initialize_Prototype() override;
	virtual			HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual			HRESULT Render() override;

private:
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer*		m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CCollider*		m_pColliderCom = { nullptr };

	_float3			m_vOriginPosition = {};
	_float3			m_vTargetPosition = {};
	_bool			m_bAction = {};
private:
	void ShakingZ(_float fCenterPosZ, _float fShakingPower);
	void StartAction(_float UntilY);
	void Check_Collision();

	HRESULT Ready_Components();
	HRESULT	Begin_RenderState();
	HRESULT	End_RenderState();
public:
	static CGiantHand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END