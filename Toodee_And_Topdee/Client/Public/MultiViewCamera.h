#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Event.h"

BEGIN(Client)

class CMultiViewCamera final : public CGameObject
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
	CMultiViewCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMultiViewCamera(const CMultiViewCamera& Prototype);
	virtual ~CMultiViewCamera() = default;

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
		m_fBackupPos = m_pTransformCom->Get_State(STATE::POSITION);
	}

private:
	CTransform* m_pTransformCom = { nullptr };
	_float4x4					m_ProjMatrix{};
	_float						m_fFovy{}, m_fAspect{}, m_fNear{}, m_fFar{};
	CRITICAL_SECTION			m_CriticalSection = {};

private:
	POINT						m_OldPoint = {};
	_bool						m_bOldKey = {};
	_bool						m_bRotating = false;
	_float						m_fTargetAngle = 12.f;
	_float						m_fCurrentAngle = {};
	_float						m_fCurrentPos = {};
	_float						m_OffsetLength;
	_float						m_ChangeSpeed = 18.f;
	CAM_TYPE					m_bType = CAM_TYPE::TOP;
	_float						m_fShaking = 0.f;
	_float3						m_fBackupPos = {};

private:
	HRESULT Ready_Components(void* pArg);
	void	CameraTestMoveInitialize();
	void	CameraTestMove(_float fTimeDelta);
	void	SetViewFlag() { m_bRotating = true; };
	void	Shaking(_float fTimeDelta);

public:
	static CMultiViewCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END