#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
END

BEGIN(Client)

class CSmall_Toodoo final : public CGameObject
{
private:
	CSmall_Toodoo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSmall_Toodoo(const CSmall_Toodoo& Prototype);
	virtual ~CSmall_Toodoo() = default;

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

	_float	m_fTotalTime = {};

private:
	HRESULT Ready_Component();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CSmall_Toodoo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END