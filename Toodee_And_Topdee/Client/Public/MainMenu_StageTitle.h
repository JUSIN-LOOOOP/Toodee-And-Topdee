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

class CMainMenu_StageTitle final : public CGameObject
{
private:
	CMainMenu_StageTitle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_StageTitle(const CMainMenu_StageTitle& Prototype);
	virtual ~CMainMenu_StageTitle() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	void				OnStageSelected(const STAGE_SELECT_EVENT& event);
	void				OnPageButtons(const MAINMENU_SWAP_EVENT& event);
	void				OnPageStage(const MAINMENU_SWAP_EVENT& event);
private:
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom_Arrow = { nullptr };
	CTexture*			m_pTextureCom_Text = { nullptr };
	CTexture*			m_pTextureCom_Number = { nullptr };

	_uint				m_iCurrentStage = { 0 };
	_bool				m_bOnceChangeStage = { false };


	_float3				m_vCurrentTextPosition = { 0.f, 1.f,		-40.f };
	_float3				m_vCurrentNumberPosition = { 5.f, 1.f,		-40.f };
	_float3				m_vCurrentArrowLeftPosition = { -9.f, 1.f,	-40.f };
	_float3				m_vCurrentArrowRightPosition = { 9.f, 1.f,	-40.f };

	_float3				m_vBaseTextPosition = { 0.f, 1.f,			14.f };
	_float3				m_vBaseNumberPosition = { 5.f, 1.f,			14.3f };
	_float3				m_vBaseArrowLeftPosition = { -9.f, 1.f,		14.f };
	_float3				m_vBaseArrowRightPosition = { 9.f, 1.f,		14.f };

	_float3				m_vFadeOutTextPosition = { 0.f, 1.f,		-40.f };
	_float3				m_vFadeOutNumberPosition = { 5.f, 1.f,		-40.f };
	_float3				m_vFadeOutArrowLeftPosition = { -9.f, 1.f,	-40.f };
	_float3				m_vFadeOutArrowRightPosition = { 9.f, 1.f,	-40.f };

	_float				m_fEasedTime = { 0.f };
	_float				m_fEasedAccumurateTime = { 0.f };
	_float				m_fEasedIntervalTime = { 1.8f };

	_bool				m_bIsStage = { false };
	_bool				m_bOncePageChange = { false };
	

private:
	HRESULT				Ready_SubscribeEvent(_uint iPlayLevel);
	void				Change_ArrowMotion(_float fTimeDelta);
	void				Change_TextMotion(_float fTimeDelta);
	void				Change_Page(_float fTimeDelta);

	HRESULT				Render_Arrow();
	HRESULT				Render_Text();

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CMainMenu_StageTitle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END