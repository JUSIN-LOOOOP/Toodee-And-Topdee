#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;

END


BEGIN(Client)

class CRedButton final : public CGameObject
{

private:
	CRedButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRedButton(const CRedButton& Prototype);
	virtual ~CRedButton() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

private:
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom_Face = { nullptr };
	CTexture*			m_pTextureCom_Button = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

	_float3				m_vPosition_Face = { };
	_float3				m_vPosition_PressedButton= { };
	_float3				m_vPosition_UnpressedButton = { };

	_bool				m_bPreviousPushed = { false };
	_bool				m_bPushed = { false };
	_uint				m_iPlayLevel = {};
	_uint				m_iTextureNumber = {};

	_uint				m_iMotionNum = { 0 };
	_float				m_fMotionIntervalTime = { 1.f };
	_float				m_fAccumulateMotionTime = { 0.f };

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CRedButton* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END