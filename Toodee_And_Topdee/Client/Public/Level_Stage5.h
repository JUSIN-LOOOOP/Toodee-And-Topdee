#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage5 final : public CLevel
{
private:
	CLevel_Stage5(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Stage5() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_MapObject(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Back(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_ColliderMap(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Cannon(const _wstring& strLayerTag);

public:
	static CLevel_Stage5* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END