#pragma once

#include "Component.h"

/* 객체들을 구성하는 정점들을 월드로 변환해주기위한 상태행렬을 보관. */
/* 월드행렬의 상태를 바꿔주기위한 다양한 함수를 제공.(앞으로간다. 회전한다. 쳐다본다.) */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	}TRANSFORM_DESC;

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) const {
		return *reinterpret_cast<const _float3*>(&m_WorldMatrix.m[ENUM_CLASS(eState)][0]);
	}

	const _float3& Get_State_Ref(STATE eState) const {
		return *reinterpret_cast<const _float3*>(&m_WorldMatrix.m[ENUM_CLASS(eState)][0]);
	}

	_float3 Get_Scaled() const {
		_float3		vRight = Get_State(STATE::RIGHT);
		_float3		vUp = Get_State(STATE::UP);
		_float3		vLook = Get_State(STATE::LOOK);
		return _float3(
			D3DXVec3Length(&vRight),
			D3DXVec3Length(&vUp),
			D3DXVec3Length(&vLook)
		);
	}
	
	const _float4x4* Get_WorldMatrix() {
		return &m_WorldMatrix;
	}

	const _float4x4* Get_WorldMatrix_Inverse() {
		return D3DXMatrixInverse(&m_WorldMatrixInverse, nullptr, &m_WorldMatrix);
	}

	void Set_State(STATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[ENUM_CLASS(eState)][0], &vState, sizeof(_float3));
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);

	void Look_At(const _float3& vTarget);
	void Move_To(const _float3& vTarget, _float fTimeDelta, _float fLimitRange = 0.f);

	void Rotation(const _float3& vAxis, _float fRadian);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);

public:
	void Bind_Matrix();

private:
	_float4x4				m_WorldMatrix = { };
	_float4x4				m_WorldMatrixInverse = {};
	_float					m_fSpeedPerSec = { };
	_float					m_fRotationPerSec = { };

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END