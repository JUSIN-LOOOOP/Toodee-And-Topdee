#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
class CCollider;
END

BEGIN(Client)

class CToodoo_Ending final : public CUIObject
{
private:
	CToodoo_Ending(LPDIRECT3DDEVICE9 pGraphic_Device);
	CToodoo_Ending(const CToodoo_Ending& Prototype);
	virtual ~CToodoo_Ending() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

	HRESULT Ready_Components();

public:
	static CToodoo_Ending* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END