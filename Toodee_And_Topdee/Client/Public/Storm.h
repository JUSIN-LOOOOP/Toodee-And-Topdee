#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include <array>


//z축 아래로 충돌처리
/* cloud 따라 움직이며 위치 계산 및 비와 번개를 관리하는 매니저 역활  */

/* 맵에 구름 1개만 생성되는 가정으로 만듦. 만약 다수의 구름이 있으면 클론울 만드는 단계에서 각각의 구름 트랜스폼을 가지고와야함. */
/* 구름 먼저 클론하고 이거 클론해야함! 순서 주의  */

BEGIN(Engine)
class CTransform;
class CCollider;
END

BEGIN(Client)

class CStorm final : public CGameObject
{
private:
	CStorm(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStorm(const CStorm& Prototype);
	virtual ~CStorm() = default;

public:
	HRESULT					Initialize_Prototype() override;
	HRESULT					Initialize(void* pArg) override;
	void					Priority_Update(_float fTimeDelta) override;
	void					Update(_float fTimeDelta) override;
	void					Late_Update(_float fTimeDelta) override;
	HRESULT					Render() override;

private:
	class CMultiViewCamera* m_pCamera = { nullptr };
	CTransform*				m_pCloudTransformCom = { nullptr };

	CTransform*				m_pTransformCom[3] = { nullptr,nullptr,nullptr };
	CCollider*				m_pColliderCom[3] = { nullptr,nullptr,nullptr };

	array<_wstring, 3>		m_strArrayTransformTag = { TEXT("Com_Transform1"), TEXT("Com_Transform2"), TEXT("Com_Transform3") };
	array<_wstring, 3>		m_strArrayColliderTag = { TEXT("Com_Collision1"), TEXT("Com_Collision2"), TEXT("Com_Collision3") };

	_float3					m_CrashAnyPosition[3] = {};
	_float3					m_CrashGroundPosition[3] = {};
	_float3					m_Toodee_StartPosition[3] = {};
	_float3					m_Topdee_StartPosition[3] = {};
	
	pair<_float, _float>	m_RainSpawnTime[3] = {};
	pair<_float, _float>	m_LightningSpawnTime[3] = {};

	DIMENSION				m_eDimension = { DIMENSION::NONE };
	_bool					m_bOnceToodee = { false };

	_float					m_fRainSplashIntervalTime = { 1.f }, m_fRainSplashAccumulateTime = { 0.f };

private:
	void					Compute_CrashSite(_float fTimeDelta);
	void					Collider_Position_Initailize();
	void					SpawnRain(_float fTimeDelta);
	void					SpawnRainSplash(_float fTimeDelta);
	void					SpawnLightning(_float fTimeDelta);



private:
	HRESULT					Ready_Components();

public:
	static CStorm* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
