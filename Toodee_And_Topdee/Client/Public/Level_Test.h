#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Test final : public CLevel
{
private:
	CLevel_Test(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Test() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_TestCube(const _wstring& strLayerTag);
	HRESULT Ready_Layer_TestCube2(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tile(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Potal(const _wstring& strLayerTag);
	HRESULT Ready_Layer_ColliderCube(const _wstring& strLayerTag);

	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);


public:
	static CLevel_Test* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END