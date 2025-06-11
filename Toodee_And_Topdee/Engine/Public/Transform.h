#pragma once

#include "Component.h"

/* ��ü���� �����ϴ� �������� ����� ��ȯ���ֱ����� ��������� ����. */
/* ��������� ���¸� �ٲ��ֱ����� �پ��� �Լ��� ����.(�����ΰ���. ȸ���Ѵ�. �Ĵٺ���.) */

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

	void Set_WorldMatrix(const _float4x4& matrix) { m_WorldMatrix = matrix; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);

	void Look_At(const _float3& vTarget);
	void Move_To(const _float3& vTarget, _float fTimeDelta, _float fLimitRange = 0.f);
	_bool MoveUntilInRange(const _float3& vTarget, _float fTimeDelta, _float fLimitRange);
	_bool Approach(const _float3& vTarget, _float fTimeDelta, _float fSpeed);
	void Rotation(const _float3& vAxis, _float fRadian);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void TurnToRadian(const _float3& vAxis, _float fRadian);
	void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);
	/* vTarget ��ġ���� vAxis �� ȸ��, fRotationDegree = �����Ҷ����� ȸ���� ����, fDistance = ���۱��� Ÿ�ٰ��� �Ÿ�*/
	_bool Spiral(const _float3& vTarget, const _float3& vAxis, _float fRotationDegree, _float fDistance, _float fTimeDelta);
	void Adjust_Scale(const _float3& vScale);
	_bool Move_To_LimitY(const _float3& vTarget, _float fTimeDelta, _float fLimitY = 0.f);
	void Set_Matrix(const _float4x4& matrix);

	void Look_At_FixUp(const _float3& vTarget);
	void Look_At_Divide(const _float3& vTarget, _float fTimeDelta);
	_bool Turn_Divide(const _float3& vAxis, _float fRadian, _float fRotationPerSec ,_float fTimeDelta);

public:
	void Bind_Matrix();

private:
	_float4x4				m_WorldMatrix = { };
	_float4x4				m_WorldMatrixInverse = {};
	_float					m_fSpeedPerSec = { };
	_float					m_fRotationPerSec = { };
	_float					m_fTotalRotation = {};
	_float3					m_movedirection = {};
	_float					m_fTotalTurnRadian = {};
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END