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

class CMainMenu_Buttons final : public CGameObject
{
private:
	CMainMenu_Buttons(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_Buttons(const CMainMenu_Buttons& Prototype);
	virtual ~CMainMenu_Buttons() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;
	
public:
	void OnPageUp(const MAINMENU_SWAP_EVENT& event);
	void OnPageDown(const MAINMENU_SWAP_EVENT& event);

private:
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom_Buttons = { nullptr };
	CTexture*			m_pTextureCom_Arrow = { nullptr };

	_int				m_iCurrentButton = { 0 };

	_float3				m_vButton_MainMenu_Position[2] = {};
	_float3				m_vButton_Stage_Position[2] = {};
	_float3				m_vButton_Position[2] = {};


	_float3				m_vButton_Size = {};
	_float				m_fArrow_OffsetX = {};
	_float3				m_vArrow_Size = {};

	_float				m_fMotionIntervalTime = { 0.05f };
	_float				m_fMotionAccumurateTime = { 0.f };
	_float				m_fMotionArrowPositionX = { 0.f };
	_float				m_bMotionArrow = { false };

	/*true - mainmenu, false - stage*/
	_bool				m_bIsMainMenu = { true };
	_bool				m_bArrivalPosition = { true };
	_float				m_easedTime = { 0.f };
	_uint				m_iPlayLevel = {};

private:
	void				Change_Motion(_float fTimeDelta);
	HRESULT				Ready_SubscribeEvent(_uint iPlayLevel);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	void				SetUp_GreenRenderState();
	void				Reset_GreenRenderState();

public:
	static CMainMenu_Buttons* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END