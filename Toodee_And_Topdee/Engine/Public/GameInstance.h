#pragma once

#include "Prototype_Manager.h"
#include "EventBus.h"

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

public:
	_float			Rand_Normal();
	_float			Rand(_float fMin, _float fMax);
	void			View_FrameRate(HWND hWnd = 0);

	//Renderer
public:
	HRESULT			Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

	//Level
public:
	HRESULT			Open_Level(_uint iLevelID, class CLevel* pNewLevel);
	const _uint		Get_CurrentLevelID();
	void			Set_CurrentLevelID(_uint ilevel);
	const _uint		Get_NextLevelID();
	void			Set_NextLevelID(_uint ilevel);

	//Prototype
public:
	HRESULT			Add_Prototype(_uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase*	Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);

	//Object
public:
	class CComponent*	Find_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	HRESULT				Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CGameObject*	Get_BackGameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag); /* 인 게임단계에서 오브젝트 찾기용 */
	class CGameObject*	Get_Ready_BackGameObject(const _wstring& strLayerTag); /* Ready_Layer_GameObject 단계에서 사용하는 오브젝트 찾기용 */
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

	//Map_Manager
public:
	HRESULT				Load_File(const _wstring& filename);
	BLOCK_INFO			Get_CurrentType();									
	HRESULT				Add_Tile(CGameObject* tile);						
	HRESULT				Get_Tile_Data(_int idx, BLOCK_INFO& block_data);
	HRESULT				Load_Initial_Data(vector<_uint>* blockData);

	//Collision
public:
	HRESULT			Add_Collider(_uint iLevelIndex, COLLIDER_SHAPE etype, class CCollider** pCollider);
	void			Delete_Collider(_uint iLevelIndex, CCollider** pCollider);
	void			Check_Collision(class CCollider* pCollider);

	//EventBus
	template <typename EventType>
	void			Subscribe(_uint iSubscribeLevel, EVENT_KEY eEventKey, function<void(const EventType&)> Callback)
					{ m_pEventBus->Subscribe(iSubscribeLevel, eEventKey, Callback); }
	void			Publish(_uint iSubscribeLevel, EVENT_KEY eEventKey, const CEvent& Event);

	//Sound
public:
	void			PlayAudio(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void			PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void			StopSound(CHANNELID eID);
	void			StopAll();
	void			SetChannelVolume(CHANNELID eID, float fVolume);

	//Pool
public:
	void					First_Push(const _wstring& strPoolTag,  class CPoolableObject* pGameObject);
	void					Push(const _wstring& strPoolTag, class CPoolableObject* pGameObject);
	class CPoolableObject*	Pop(_uint iPrototypeLevelIndex, const _wstring& strPoolTag);

	//Effect
public:
	void					Add_PSystem(class CPSystem* pPSystem, const _wstring& strEffectTag);
	void					Set_Active(const _wstring& strEffectTag, void* pArg = nullptr);
	//Dimension
public:
	void			Change_Dimension(DIMENSION eDimension) 
	{
		if(DIMENSION::CHANGE != m_eCurrentDimension)
		m_ePreviousDimension = m_eCurrentDimension;
		m_eCurrentDimension = eDimension; 
	}
	DIMENSION		Get_CurrentDimension() { return m_eCurrentDimension; }
	DIMENSION		Get_PreviousDimension() { return m_ePreviousDimension; }

	//Key Count
private:
	_uint m_iTotalKey = {};

public:
	void Reset_KeyCount() { m_iTotalKey = 0; }
	void Add_Key() { m_iTotalKey++; }
	_uint GetKeyCount() { return m_iTotalKey; }

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CKey_Manager*			m_pKey_Manager = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CCollision_Manager*	m_pCollision_Manager = { nullptr };

	class CMap_Manager*			m_pMap_Manager = { nullptr };
	class CSound_Manager*		m_pSound_Manager = { nullptr };
	class CPool_Manager*		m_pPool_Manager = { nullptr };
	class CEffectManager*		m_pEffect_Manager = { nullptr };

	CEventBus* m_pEventBus = { nullptr };

private:
	DIMENSION					m_ePreviousDimension = { DIMENSION::NONE };
	DIMENSION					m_eCurrentDimension = { DIMENSION::NONE };
	_uint						m_iCurrentLevelID = { 0 };
	_uint						m_iNextLevelID = { 0 };
	_uint						m_iFPS = { 0 };
	_ulong						m_ulIntervalTime_FPS = { GetTickCount() };
	TCHAR						m_szFPS[16] = {};

public:
	void Release_Engine();
	virtual void Free() override;
};

END