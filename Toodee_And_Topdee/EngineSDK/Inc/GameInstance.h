#pragma once

#include "Prototype_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

	//Engine
public:
	HRESULT			Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut);
	void			Update_Engine(_float fTimeDelta);
	HRESULT			Clear_Resources(_uint iClearLevelID);

	void			Render_Begin(D3DXCOLOR Color);
	HRESULT			Draw();
	void			Render_End(HWND hWnd = 0);

	//Renderer
public:
	HRESULT			Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

	//Level
public:
	HRESULT			Open_Level(_uint iLevelID, class CLevel* pNewLevel);

	//Prototype
public:
	HRESULT			Add_Prototype(_uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase*	Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);

	//Object
public:
	HRESULT			Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	
	//key
public:
	_bool			Key_Pressing(int _iKey);
	_bool			Key_Up(int _iKey);
	_bool			Key_Down(int _iKey);

	//Timer
public:
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
	void			Compute_TimeDelta(const _wstring& strTimerTag);

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CKey_Manager*			m_pKey_Manager = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };



public:
	void Release_Engine();
	virtual void Free() override;
};

END