#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
class CShader;
END

BEGIN(Client)

class CEnding_Flash final : public CUIObject
{
private:
	CEnding_Flash(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnding_Flash(const CEnding_Flash& Prototype);
	virtual ~CEnding_Flash() = default;

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
	CShader* m_pShader = { nullptr };

	_bool			m_bRender = {};
	_float			m_fTotalTime = {};

private:
	HRESULT Begin_RenderState();
	HRESULT Begin_ShaderState();
	HRESULT End_RenderState();
	HRESULT End_ShaderState();

	HRESULT Ready_Components();

public:
	static CEnding_Flash* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END