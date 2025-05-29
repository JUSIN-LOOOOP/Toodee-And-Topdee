#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTextureUI final : public CGameObject
{
private:
	CTextureUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTextureUI(const CTextureUI& Prototype);
	virtual ~CTextureUI() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	CTransform*					m_pTransformCom = { nullptr };

private :
	_uint						m_iTextureIdx = 0;
	_float4x4					m_ViewMatrix{}, m_ProjMatrix{};

private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();


public:
	static CTextureUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END