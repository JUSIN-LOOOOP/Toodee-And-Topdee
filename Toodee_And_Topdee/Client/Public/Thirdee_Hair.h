#pragma once
#include "Thirdee_Part.h"

BEGIN(Client)

class CThirdee_Hair final : public CThirdee_Part
{
private:
	CThirdee_Hair(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	CThirdee_Hair(const CThirdee_Hair& Prototype);
	virtual ~CThirdee_Hair() = default;

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
	static CThirdee_Hair* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END