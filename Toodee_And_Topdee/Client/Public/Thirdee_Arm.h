#pragma once
#include "Client_Defines.h"
#include "Thirdee_Part.h"

BEGIN(Client)

class CThirdee_Arm final : public CThirdee_Part
{
private:
	CThirdee_Arm(LPDIRECT3DDEVICE9 pGraphic_Deivce);
	CThirdee_Arm(const CThirdee_Arm& Prototype);
	virtual ~CThirdee_Arm() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void AnimUpdate(_float fTimeDelta, PLAYERSTATE eState) override;

private:
	_float	m_fRadianPerSec = {};
	_float	m_fAnimTime = {};
	_float	m_fCurrentAnimRadian = {};

public:
	static CThirdee_Arm* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END