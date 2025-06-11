#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Event.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CLoadingScreen final : public CGameObject
{
private:
	CLoadingScreen(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLoadingScreen(const CLoadingScreen& Prototype);
	virtual ~CLoadingScreen() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	void				OnStartScreen(const LOADINGSCREEN_EVENT& event);

private:
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };

	_float3				m_vPosition = { 0.f, 2.f, 0.f };
	_float3				m_vScale = { 128.f, 128.f, 128.f };

	_bool				m_bStartScreen = { false };
	_bool				m_bEndScreen = {false };
	_bool				m_bFadeIn = { true };

	_float				m_fIntervalTime = { 0.07f };
	_float				m_fAccumurateTime = { 0.f };
	_float				m_FadeAccumurateTime = { 0.f };
	_uint				m_iframeKey = { 0 };

	ID3DXFont*			m_pFont = { nullptr };

private:
	void				Motion_Change(_float fTimeDelta);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	HRESULT				Ready_SubscribeEvent(_uint iPlayLevel);

public:
	static CLoadingScreen* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END