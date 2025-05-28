#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END


BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT		Initialize();
	void		Update(_float fTimeDelta);
	HRESULT		Render();

private:
	CGameInstance*		m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

private:
	HRESULT Ready_Default_Setting();
	HRESULT Ready_Prototype_ForStatic();
	HRESULT Ready_Prototype_ForStatic_Parts();
	HRESULT Start_Level(LEVEL eStartLevelID);


public:
	static CMainApp* Create();
	virtual void Free() override;
};


END

