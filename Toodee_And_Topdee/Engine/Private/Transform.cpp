#include "Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device  }
{
}

CTransform::CTransform(const CTransform& Prototype)
	:CComponent( Prototype )
	, m_WorldMatrix{ Prototype.m_WorldMatrix }
{
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vLook = Get_State(STATE::LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vLook = Get_State(STATE::LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vRight = Get_State(STATE::RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vRight = Get_State(STATE::RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vUp = Get_State(STATE::UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vUp = Get_State(STATE::UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * m_fSpeedPerSec;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Down_UntilY(_float fTimeDelta, _float UntilY)
{

	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vUp = Get_State(STATE::UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta * m_fSpeedPerSec;

	if (vPosition.y <= UntilY)
		vPosition.y = UntilY;


	Set_State(STATE::POSITION, vPosition);
}


void CTransform::Look_At(const _float3& vTarget)
{
	_float3 vScale = Get_Scaled();
	_float3 vRight{}, vUp{}, vLook{};

	vLook = vTarget - Get_State(STATE::POSITION);

	_float3 vUpDir = { 0.f, 1.f, 0.f };
	D3DXVec3Cross(&vRight, &vUpDir, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(STATE::RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(STATE::UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(STATE::LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::Move_To(const _float3& vTarget, _float fTimeDelta, _float fLimitRange)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vMoveDir = vTarget - vPosition;

	if (fLimitRange <= D3DXVec3Length(&vMoveDir))
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE::POSITION, vPosition);
	}
}

_bool CTransform::MoveUntilInRange(const _float3& vTarget, _float fTimeDelta, _float fLimitRange)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vMoveDir = vTarget - vPosition;

	if (fLimitRange <= D3DXVec3Length(&vMoveDir))
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE::POSITION, vPosition);
		return false;
	}
	else
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE::POSITION, vPosition);
	}
	
	return true;
}

_bool CTransform::Approach(const _float3& vTarget, _float fTimeDelta, _float fSpeed)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vMoveDir = vTarget - vPosition;

	_float fDistance = D3DXVec3Length(&vMoveDir);

	//if (fLimitRange <= D3DXVec3Length(&vMoveDir))
	{
		if (D3DXVec3Length(&vMoveDir) <= 0.001f)
		{
			return true;
		}

		//_float fDistanceX = vMoveDir.x;
		//_float fDistanceY = vMoveDir.y;
		//_float fDistanceZ = vMoveDir.z;

		_float3 vMove = *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * fSpeed * fTimeDelta;

		if (D3DXVec3Length(&vMove) >= fDistance)
		{
			//if (abs(vMove.x) >= abs(fDistanceX))
			//	vMove.x = fDistanceX;
			//
			//if (abs(vMove.y) >= abs(fDistanceY))
			//	vMove.y = fDistanceY;
			//
			//if (abs(vMove.z) >= abs(fDistanceZ))
			//	vMove.z = fDistanceZ;
			//
			//vPosition += vMove;

			Set_State(STATE::POSITION, vTarget);

			return true;
		}

		vPosition += vMove;
		Set_State(STATE::POSITION, vPosition);
	}
	return false;
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3 vScale = Get_Scaled();

	_float3 vRight =	_float3(1.f, 0.f, 0.f) * vScale.x;
	_float3 vUp =		_float3(0.f, 1.f, 0.f) * vScale.y;
	_float3 vLook =		_float3(0.f, 0.f, 1.f) * vScale.z;

	_float3 NewvAxis = {};
	_float4x4 rotationMatrix{};
	D3DXVec3Normalize(&NewvAxis, &vAxis);

	D3DXMatrixRotationAxis(&rotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &rotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &rotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &rotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	_float3 vRight = Get_State(STATE::RIGHT);
	_float3 vUp = Get_State(STATE::UP);
	_float3 vLook = Get_State(STATE::LOOK);

	_float4x4 rotationMatrix{};

	D3DXMatrixRotationAxis(&rotationMatrix, &vAxis, m_fRotationPerSec* fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &rotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &rotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &rotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::TurnToRadian(const _float3& vAxis, _float fRadian)
{
	_float3 vRight = Get_State(STATE::RIGHT);
	_float3 vUp = Get_State(STATE::UP);
	_float3 vLook = Get_State(STATE::LOOK);

	_float4x4 rotationMatrix{};

	D3DXMatrixRotationAxis(&rotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &rotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &rotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &rotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::Look_At_FixUp(const _float3& vTarget)
{
	_float3 vScale = Get_Scaled();
	
	_float3 vPosition = Get_State(STATE::POSITION);

	_float3 vLook = vTarget - vPosition;
	_float3 vUp = Get_State(STATE::UP);
	_float3 vRight = {};

	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(STATE::RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(STATE::UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(STATE::LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

void CTransform::Look_At_Divide(const _float3& vTarget, _float fTimeDelta)
{
	_float3 vScale = Get_Scaled();

	// 현재 방향
	_float3 vCurrentLook = Get_State(STATE::LOOK);
	D3DXVec3Normalize(&vCurrentLook, &vCurrentLook);

	// 목표 방향
	_float3 vTargetDir = vTarget - Get_State(STATE::POSITION);
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);

	// 두 방향의 각도
	float fDot = D3DXVec3Dot(&vCurrentLook, &vTargetDir);
	
	fDot = max(min(fDot, 1.f), -1.f);

	float fRotationRadian = min(1.f - fDot, m_fRotationPerSec * fTimeDelta);

	// 회전 축 계산
	_float3 vRotateAxis;
	D3DXVec3Cross(&vRotateAxis, &vCurrentLook, &vTargetDir);
	D3DXVec3Normalize(&vRotateAxis, &vRotateAxis);

	if (D3DXVec3Length(&vRotateAxis) < 0.0001f)
		return;

	// 회전 행렬 생성
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &vRotateAxis, fRotationRadian);

	// 새 방향 계산
	D3DXVECTOR3 vNewLook;
	D3DXVec3TransformNormal(&vNewLook, &vCurrentLook, &matRot);
	vNewLook = *D3DXVec3Normalize(&vNewLook, &vNewLook);

	// Right, Up 벡터 재계산
	_float3 vUpDir = { 0.f, 1.f, 0.f };
	_float3 vRight;
	D3DXVec3Cross(&vRight, &vUpDir, &vNewLook);
	vRight = *D3DXVec3Normalize(&vRight, &vRight);

	_float3 vUp;
	D3DXVec3Cross(&vUp, &vNewLook, &vRight);
	vUp = *D3DXVec3Normalize(&vUp, &vUp);

	Set_State(STATE::RIGHT, vRight * vScale.x);
	Set_State(STATE::UP, vUp * vScale.y);
	Set_State(STATE::LOOK, vNewLook * vScale.z);
}

_bool CTransform::Turn_Divide(const _float3& vAxis, _float fRadian, _float fRotationPerSec, _float fTimeDelta)
{
	_float fTurnRadian = fRotationPerSec * fTimeDelta;

	fTurnRadian = (fRadian > 0.f) ? fTurnRadian : (fTurnRadian * -1.f);

	if ((fRadian >= 0.f && m_fTotalRotation + fTurnRadian >= fRadian) ||
		(fRadian < 0.f && m_fTotalRotation + fTurnRadian <= fRadian))
	{
		fTurnRadian = fRadian - m_fTotalRotation;
		TurnToRadian(vAxis, fTurnRadian);

		m_fTotalRotation = 0.f;
		return true;
	}


	m_fTotalRotation += fTurnRadian;

	TurnToRadian(vAxis, fTurnRadian);

	return false;
}


void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	_float3 vRight{ Get_State(STATE::RIGHT) }, vUp{ Get_State(STATE::UP) }, vLook{ Get_State(STATE::LOOK) };

	Set_State(STATE::RIGHT, *D3DXVec3Normalize(&vRight ,&vRight)* fScaleX);
	Set_State(STATE::UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);
	Set_State(STATE::LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);
}

_bool CTransform::Spiral(const _float3& vTarget, const _float3& vAxis, _float fRotationDegree, _float fDistance,_float fTimeDelta)
{
	_float3 vPosition = Get_State(STATE::POSITION);


	_float4x4 rotationMatrix{};

	//회전 속도
	_float fRotationPerSec = D3DXToRadian(fRotationDegree);

	//지금까지 회전한 각도
	m_fTotalRotation += fRotationPerSec * fTimeDelta;

	//회전할 위치벡터
	_float3 vRotatePosition = vPosition - vTarget;

	//위치벡터 회전
	D3DXMatrixRotationAxis(&rotationMatrix, &vAxis, fRotationPerSec * fTimeDelta);
	D3DXVec3TransformNormal(&vRotatePosition, &vRotatePosition, &rotationMatrix);

	//원래 위치벡터를 회전한 위치벡터로 갱신
	vPosition = vTarget + vRotatePosition;

	
	//방향벡터 구하기
	
	_float3 vMoveDir = vTarget - vPosition;

	//타겟과의 거리와 회전할 각도를 통해 스피드 구하기 , Distance Toodee가 음수 들어가서 abs
	_float fSpeedPerSec = abs(fDistance) * (fRotationPerSec / (2.f * D3DX_PI));
	_float3 vMove = *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * fSpeedPerSec * fTimeDelta;

	vPosition += vMove;

	Set_State(STATE::POSITION, vPosition);

	//목표만큼 각도보다 회전 했다면 true
	if (m_fTotalRotation >= fRotationPerSec)
		return true;

	return false;
}

void CTransform::Adjust_Scale(const _float3& vScale)
{
	_float3 vRight{ Get_State(STATE::RIGHT) }, vUp{ Get_State(STATE::UP) }, vLook{ Get_State(STATE::LOOK) };

	Set_State(STATE::RIGHT, vRight * vScale.x);
	Set_State(STATE::UP, vUp * vScale.y);
	Set_State(STATE::LOOK, vLook * vScale.z);
}

_bool CTransform::Move_To_LimitY(const _float3& vTarget, _float fTimeDelta, _float fLimitY)
{
	_float3 vPosition = Get_State(STATE::POSITION);
	_float3 vMoveDir = vTarget - vPosition;
	_float fDistanceXZ = vMoveDir.x * vMoveDir.x + vMoveDir.z * vMoveDir.z;

	D3DXVec3Normalize(&vMoveDir, &vMoveDir);



	if (fDistanceXZ >= fLimitY)
	{
		vPosition.x += vMoveDir.x * m_fSpeedPerSec * fTimeDelta;
		vPosition.z += vMoveDir.z * m_fSpeedPerSec * fTimeDelta;
		Set_State(STATE::POSITION, vPosition);
		return false;
	}
	else
		return true;
}
		
void CTransform::Set_Matrix(const _float4x4& matrix)
{
	_float3 vRight = _float3(1.f, 0.f, 0.f);
	_float3 vUp = _float3(0.f, 1.f, 0.f);
	_float3 vLook = _float3(0.f, 0.f, 1.f);
	_float3 vScale = Get_Scaled();

	D3DXVec3TransformNormal(&vRight, &vRight, &matrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &matrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &matrix);

	Set_State(STATE::RIGHT, vRight * vScale.x);
	Set_State(STATE::UP, vUp * vScale.y);
	Set_State(STATE::LOOK, vLook * vScale.z);
}

void CTransform::Bind_Matrix()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTransform"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
