#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* ��ü, ���ҽ�, ����, ���̴� ������->�ڿ� �غ� ��ġ�� ���� */

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

	HRESULT Ready_PigTexture();

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphicDev, LEVEL eNextLevelID);
	virtual void Free() override;

};

END