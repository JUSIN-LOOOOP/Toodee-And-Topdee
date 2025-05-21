#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT		Initialize();
	virtual void		Update(_float fTimeDelta) override;
	virtual HRESULT		Render()override;

private:


public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
};

END