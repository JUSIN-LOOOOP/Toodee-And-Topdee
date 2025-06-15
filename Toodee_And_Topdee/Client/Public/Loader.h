#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 객체, 리소스, 사운드, 쉐이더 원본들->자원 준비를 거치는 과정 */

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final: public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoader() = default;

public:
	HRESULT		Initailize(LEVEL eLevel);
	HRESULT		Loading();

	bool		IsFinished() { return m_isFinished; }
	void		Show_LoadingText() { SetWindowText(g_hWnd, m_szLoadingText); };

private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	CGameInstance*					m_pGameInstance = { nullptr };

	LEVEL							m_eNextLevelID = { LEVEL::LEVEL_END };
	bool							m_isFinished = { false };
	_tchar							m_szLoadingText[MAX_PATH] = {};

	HANDLE							m_hThread = {};
	CRITICAL_SECTION				m_CriticalSection = {};

private:
	HRESULT Loading_For_Logo_Level();
	HRESULT Loading_For_GamePlay_Level();
	HRESULT Loading_For_MapEdit_Level();
	HRESULT Loading_For_Stage1();
	HRESULT Loading_For_Stage2();
	HRESULT Loading_For_Stage3();
	HRESULT Loading_For_Stage4();
	HRESULT Loading_For_Stage5();
	HRESULT Loading_For_Stage6();
	HRESULT Loading_For_StageBoss();
	HRESULT Loading_For_FinalBoss01();
	HRESULT Loading_For_FinalBoss02();
	HRESULT Loading_For_FinalBoss03();
	HRESULT Loading_For_Ending();
	HRESULT Loading_For_Dialogue();

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphicDev, LEVEL eNextLevelID);
	virtual void Free() override;

};

END