#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CCollider;
END


BEGIN(Client)

class CColliderMap_Object final : public CGameObject
{
public:
	typedef struct ColliderMapDescription
	{
		_float3 vPosition;
		_float3 vScale;
	}COLLIDER_MAP_DESC;

private:
	CColliderMap_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	CColliderMap_Object(const CColliderMap_Object& Prototype);
	virtual ~CColliderMap_Object() = default;

public:
	virtual								HRESULT Initialize_Prototype() override;
	virtual								HRESULT Initialize(void* pArg) override;
	virtual void						Priority_Update(_float fTimeDelta) override;
	virtual void						Update(_float fTimeDelta) override;
	virtual void						Late_Update(_float fTimeDelta) override;
	virtual								HRESULT Render() override;

private:
	CTransform*					m_pTransformCom = { nullptr };
	CCollider*					m_pColliderCom = { nullptr };

	_float3								m_vPosition = { };
	_float3								m_vScale = { };
	_bool								m_isColliderRender = { true };

private:
	HRESULT								Ready_Components();

public:
	static CColliderMap_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CColliderMap_Object*  Clone(void* pArg) ;
	virtual void	Free() override;


};

END