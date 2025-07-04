#pragma once
#include "Client_Defines.h"
#include "Gameobject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CHole : public CGameObject
{
protected:
	CHole(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHole(const CHole& Prototype);
	virtual ~CHole() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_Dead();

private: 
	HRESULT Ready_Components();

protected:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_uint	m_iTextureIdx = {};
	_bool m_bDead = {};

public:
	static CHole* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

