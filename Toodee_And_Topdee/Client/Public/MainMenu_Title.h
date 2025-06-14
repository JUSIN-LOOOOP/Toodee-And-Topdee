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

class CMainMenu_Title final : public CGameObject
{
private:
	CMainMenu_Title(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_Title(const CMainMenu_Title& Prototype);
	virtual ~CMainMenu_Title() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_float						m_fMotionSize = { 0.f };
	_float						m_bMotionSize = { false };
	_float						m_fMotionPosition = { 0.f };
	_float						m_bMotionPosition = { false };
	_float						m_fMotionIntervalTime = { 0.1f };
	_float						m_fAccumulateMotionTime = { 0.f };

	_uint						m_iPlayLevel = {};
	_bool						m_bIsMeinMenu = { false };
	_bool						m_bPageOnceChange = { false };


// 기본 설정값들
	//실제 텍스쳐 픽셀위치 
	_float				m_fTextureHeight_InPixel = { 64.f }; //텍스쳐 전체 높이
	_float				m_fTextTopMargin_InPixel = { 17.f };
	_float				m_fTextBottomPos_InPixel = { 45.f };
	_float				m_fShadowTopMargin_InPixel = { 28.f };
	_float				m_fShadowBottomPos_InPixel = { 34.f };

	_float				fTextHeight = { 10.f };      // 텍스트 높이
	_float				fTextWidth = { 30.f };;      // 텍스트 너비
	_float				fAndSize = { 16.f };         // AND 텍스트 크기
	_float				fSpacing = { 0.f };         // 텍스트 간 간격

	_float				fToodeeZ = { 15.f };
	_float				fAndZ = { 11.f };
	_float				fTopdeeZ = { 10.f };

	_float				fBaseY = { 2.f };

	_float				fPageButtonPositionZ = { 0.f };
	_float				fPagePositionZ = { 0.f };
	_float				fPageStagePositionZ = { 36.f };
	_float				fEasedTime = { 0.f };


private:
	void				Change_Page(_float fTimeDelta);
	HRESULT				Ready_SubscribeEvent(_uint iPlayLevel);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();
	void				Change_Motion(_float fTimeDelta);


public:
	static CMainMenu_Title* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END