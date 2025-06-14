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

class CMainMenu_Portals final : public CGameObject
{
private:
	CMainMenu_Portals(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_Portals(const CMainMenu_Portals& Prototype);
	virtual ~CMainMenu_Portals() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	void OnPageButtons(const MAINMENU_SWAP_EVENT& event);
	void OnPageStage(const MAINMENU_SWAP_EVENT& event);

private:
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom_Portal = { nullptr };
	CTexture*			m_pTextureCom_StageImage = { nullptr };

	// object Position
	_float3				m_vPosition[8];
	_float3				m_vPageStagePosition[8];
	_float3				m_vPageButtonPosition[8];
	_float				m_fIntervalX = { 20.f };
	_float				m_fEasedTime = { 0.f };

	//portal color
	D3DCOLOR			m_vColor[3][8];

	//stage selected
	_uint				m_iCurSelected = { 0 };
	//_float				m_fIntervalSeletedStage = { 0.3f };
	//_float				m_fAccumulateSeletedStage = { 0.3f };
	_bool				m_bChangeStage = { false };
	//_bool				m_bOnceChangeStage = { false };

	//portal motion
	_uint					m_iMotionNum[3] = {0,3,6};
	_float					m_fMotionIntervalTime = { 0.1f };
	_float					m_fAccumulateMotionTime = { 0.f };

	//event
	_bool				m_bIsStage = { false };
	_bool				m_bPageOnceChange = { false };
	_float				m_fEasedPageTime = { 0.f };



private:
	HRESULT				Ready_SubscribeEvent(_uint iPlayLevel);
	HRESULT				Render_Portal();
	HRESULT				Render_StageImage();
	void				Change_Page(_float fTimeDelta);
	void				Apply_Selected_Position(_float fTimeDelta);
	void				Change_Motion(_float fTimeDelta);


private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	void				SetUp_ColorRenderState(_uint iNum, _uint iStage);
	void				Reset_ColorRenderState();

public:
	static CMainMenu_Portals* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END