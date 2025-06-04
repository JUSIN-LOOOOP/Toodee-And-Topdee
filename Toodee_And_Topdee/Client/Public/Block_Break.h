#pragma once
#include "Client_Defines.h"
#include "Block.h"
#include "Event.h"

BEGIN(Client)

class CBlock_Break final : public CBlock 
{
private:
	CBlock_Break(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Break(const CBlock_Break& Prototype);
	virtual ~CBlock_Break() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void StepOn(const BLOCKBREAKEVENT& Event);
	
private:
	_uint m_iPlayLevel = {};

	_bool m_bIsStepOn = {};
	_float m_fBreakDelay = {};
	_float m_fCurrentBreakTime = {};

	//Shaking
	_float3 m_vCenterPosition = {};
	_float m_fShakingPower = {};
private:
	_bool Compute_Near(const _float3& vOtherPosition);
	_bool IsNearBlock(_float3 vPosition);

	void Shaking();

	HRESULT Ready_Components();
	HRESULT Ready_SubscribeEvent(_uint iPlayerLevel);
	void	SetUp_RenderState();
	void	Reset_RenderState();

public:
	static CBlock_Break* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END

