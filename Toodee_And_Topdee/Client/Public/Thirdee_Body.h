#pragma once

#include "Client_Defines.h"
#include "Thirdee_Part.h"

BEGIN(Client)

class CThirdee_Body final : public CThirdee_Part
{
private:
	CThirdee_Body(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	CThirdee_Body(const CThirdee_Body& Prototype);
	virtual ~CThirdee_Body() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void AnimUpdate(_float fTimeDelta, PLAYERSTATE eState) override;

private:
	HRESULT Ready_Parts();

public:
	static CThirdee_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END