#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)

class CPlayer_Topdee;

class CTileOutline final : public CGameObject 
{
private:
	CTileOutline(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTileOutline(const CTileOutline& Prototype);
	virtual ~CTileOutline() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CPlayer_Topdee*		m_pOwnerPlayer = { nullptr };

	_uint					m_iMaxAnimCount = {};
	_uint					m_iCurrentAnimCount = {};
	_float					m_fAnimDelay = {};
	_float					m_fAnimTime = {};

private:
	HRESULT Ready_Components();
	HRESULT BeginRenderState();
	HRESULT EndRenderState();

	void UpdateAnim(_float fTimeDelta);

public:
	static CTileOutline* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END