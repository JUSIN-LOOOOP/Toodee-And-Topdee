#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "Transform.h"

BEGIN(Client)


class CLevel_Map final  : public CLevel
{
private:
	CLevel_Map(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Map() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual void		Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	HRESULT				Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT				Ready_Layer_Tile(const _wstring& strLayerTag);
	HRESULT				Ready_Layer_TextureUI(const _wstring& strLayerTag);


public:
	static CLevel_Map* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;

};

END