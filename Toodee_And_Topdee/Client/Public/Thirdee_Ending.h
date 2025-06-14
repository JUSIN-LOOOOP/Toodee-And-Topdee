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

class CThirdee_Ending final : public CUIObject
{
private:
	CThirdee_Ending(LPDIRECT3DDEVICE9 pGraphic_Device);
	CThirdee_Ending(const CThirdee_Ending& Prototype);
	virtual ~CThirdee_Ending() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer*		m_pVIBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CShader*		m_pShader = { nullptr };

	_float			m_fTotalTime = {};

private:
	HRESULT Begin_RenderState();
	HRESULT Begin_ShaderState();
	HRESULT End_RenderState();
	HRESULT End_ShaderState();

	HRESULT Ready_Components();

public:
	static CThirdee_Ending* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END