#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Client)

/*
	[Level에서 세팅할 CAMERA_DESC 구조체 값]
 
	CameraDesc.vEye = _float3(0.f, 250.f, 0.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 1.f );

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 10.f;
	CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);
	CameraDesc.fMouseSensor = 0.3f;
*/

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

	_float4x4	Get_ProjMatrix() { return m_ProjMatrix; };
	void		ChangeView(_float fTimeDelta);

	HANDLE							m_hThread = {};

private:
	CTransform* m_pTransformCom = { nullptr };
	_float4x4					m_ProjMatrix{};
	_float						m_fFovy{}, m_fAspect{}, m_fNear{}, m_fFar{};
	CRITICAL_SECTION			m_CriticalSection = {};

private:
	POINT						m_OldPoint = {};
	_bool						m_bOldKey = {};
	_bool						m_bRotating = false;
	_float						m_fTargetAngle = 7.f;
	_float						m_fCurrentAngle = {};
	_float						m_fCurrentPos = {};
	_float						m_OffsetLength;
	_float						m_ChangeSpeed = 13.f;
	CAM_TYPE					m_bType = CAM_TYPE::TOP;

private:
	HRESULT Ready_Components(void* pArg);
	void	SetViewFlag() { m_bRotating = true; };

public:
	static CMultiViewCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END