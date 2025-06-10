#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Event.h"

BEGIN(Client)

class CFPSCamera final : public CGameObject
{
public:
	enum class CAM_TYPE { TOP, QURTER };
	typedef struct tagCameraDesc : CTransform::TRANSFORM_DESC
	{
		_float3						vEye{}, vAt{};
		_float						fFovy{}, fNear{}, fFar{};
		_float						fMouseSensor{};
	}CAMERA_DESC;

private:
	CFPSCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFPSCamera(const CFPSCamera& Prototype);
	virtual ~CFPSCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_float4x4	Get_ProjMatrix() { return m_ProjMatrix; }
	void		ChangeView(_float fTimeDelta);

	_float3		Get_LookDirection()
	{
		_float3 dir = _float3(0.f, 0.f, 0.f) - m_pTransformCom->Get_State(STATE::POSITION);
		D3DXVec3Normalize(&dir, &dir);
		return dir;
	}

	void		SetShaking(const SHAKING& Event)
	{
		m_fShaking = Event.fTime; 
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	_float4x4					m_ProjMatrix{};
	_float						m_fFovy{}, m_fAspect{}, m_fNear{}, m_fFar{};
	CRITICAL_SECTION			m_CriticalSection = {};
	CTransform* m_pPlayer		= { nullptr };
	_float3						m_offset =  { - 10.f, 10.f, 0.f };
	_float4x4					m_localTras{};

private:
	POINT						m_OldPoint = {};
	_bool						m_bOldKey = {};
	_bool						m_bRotating = false;
	_float						m_fTargetAngle = 90.f;
	_float						m_fCurrentAngle = {};
	_float						m_fCurrentPos = {};
	_float						m_OffsetLength;
	_float						m_ChangeSpeed = 200.f;
	CAM_TYPE					m_bType = CAM_TYPE::TOP;
	_float						m_fShaking = 0.f;

private:
	HRESULT Ready_Components(void* pArg);
	void	CameraTestMoveInitialize();
	void	CameraTestMove(_float fTimeDelta);
	void	SetViewFlag() { m_bRotating = true; };
	void	Shaking(_float fTimeDelta);

public:
	static CFPSCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END