#pragma once
#include "Client_Defines.h"
#include "Thirdee_Part.h"

BEGIN(Client)

class CThirdee_Head final : public CThirdee_Part
{
private:
	CThirdee_Head(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	CThirdee_Head(const CThirdee_Head& Prototype);
	virtual ~CThirdee_Head() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void AnimUpdate(_float fTimeDelta, PLAYERSTATE eState) override;

private:
	HRESULT Ready_Hair();


public:
	static CThirdee_Head* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END