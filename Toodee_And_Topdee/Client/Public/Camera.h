#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Client)

class CCamera final : public CGameObject
{
public:
	typedef struct tagCameraDesc : CTransform::TRANSFORM_DESC
	{
		_float3						vEye{}, vAt{};
		_float						fFovy{}, fNear{}, fFar{};
		_float						fMouseSensor{};
	}CAMERA_DESC;
private:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };

	_float4x4					m_ProjMatrix{};

	_float						m_fFovy{}, m_fAspect{}, m_fNear{}, m_fFar{};

private:
	POINT						m_OldPoint = {};
	_float						m_fMouseSensor = {};

private: 
	HRESULT Ready_Components(void* pArg);


public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END