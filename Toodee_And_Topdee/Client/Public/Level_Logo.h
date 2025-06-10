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
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Spark(const _wstring& strLayerTag);
	HRESULT Ready_Layer_CenterLine(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Buttons(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Title(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Portal(const _wstring& strLayerTag);
	HRESULT Ready_Layer_StageTitle(const _wstring& strLayerTag);



public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;
};

END