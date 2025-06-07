#pragma once
#include "Client_Defines.h"
#include "StageBoss_limb.h"

BEGIN (Client)

class CStageBoss_Hand : public CStageBoss_limb
{
private:
	CStageBoss_Hand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStageBoss_Hand(const CStageBoss_Hand& Prototype);
	virtual ~CStageBoss_Hand() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg);
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();

public:
	static CStageBoss_Hand* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END