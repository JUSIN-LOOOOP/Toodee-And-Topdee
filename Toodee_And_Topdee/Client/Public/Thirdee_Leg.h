#pragma once
#include "Client_Defines.h"
#include "Thirdee_Part.h"

BEGIN(Client)

class CThirdee_Leg final : public CThirdee_Part
{
private:
	CThirdee_Leg(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	CThirdee_Leg(const CThirdee_Leg& Prototype);
	virtual ~CThirdee_Leg() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void AnimUpdate(_float fTimeDelta, PLAYERSTATE eState) override;
private:


public:
	static CThirdee_Leg* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END