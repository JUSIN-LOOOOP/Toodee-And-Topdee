#pragma once
#include "Client_Defines.h"
#include "PoolableObject.h"
#include "Event.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPlayerChangeEffect final : public CPoolableObject
{
private:
	CPlayerChangeEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerChangeEffect(const CPlayerChangeEffect& Prototype);
	virtual ~CPlayerChangeEffect() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

public:
	HRESULT				Initialize_Pool(void* pArg) override;
	void				Get_PlayersPosition_Event(const PLAYERSPOSITION_EVENT& event);

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTransform*					m_pTransformCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };
	
	_float3						m_vCurPosition = {};
	_float3						m_vStartPosition = {};
	_float3						m_vGoalPosition = {};
	_bool						m_bStartEffect = { false };

	_uint						m_iMotionNum = { 0 };
	_float						m_fMotionIntervalTime = { 0.15f };
	_float						m_fAccumulateMotionTime = { 0.f };

	_float						m_fTravelTime = { 1.5f };
	_float						m_fTravelAccumurateTime = { 0.f };

	D3DCOLOR					m_Color = {};
	_float						m_fHue = 0.0f;
	_float						m_fColorSpeed = 120.0f; 

	//Player Change Event
	CTransform*					m_pTransform_Toodee = { nullptr };
	CTransform*					m_pTransform_Topdee = { nullptr };

private:
	HRESULT						Ready_SubscribeEvent(_uint iPlayerLevel = ENUM_CLASS(LEVEL::LEVEL_STATIC));
	HRESULT						Ready_Components();
	void						SetUp_RenderState();
	void						Reset_RenderState();
	void						Change_Motion(_float fTimeDelta);
	void						Change_Color(_float fTimeDelta);
	void						Go_Goal(_float fTimeDelta);
	D3DCOLOR					HSVtoARGB(_float h, _float s, _float v, _float a = 1.0f);

public:
	static CPlayerChangeEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;



	

};

END