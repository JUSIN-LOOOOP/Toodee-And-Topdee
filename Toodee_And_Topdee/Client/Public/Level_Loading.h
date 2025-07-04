#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT		Initialize(LEVEL eNextLevelID);
	virtual void		Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	LEVEL				m_eNextLevelID = { LEVEL::LEVEL_END };
	class				CLoader* m_pLoader = { nullptr };


private:
	HRESULT	Ready_GameObjects();
	HRESULT	Ready_LoadingThread();


public:
	static CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LEVEL eNextLevelID);
	virtual void Free() override;
};

END
