#include "Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent { pGraphic_Device  }
{
}

CTransform::CTransform(const CTransform& Prototype)
	:CComponent{ Prototype }
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

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3 vScale = Get_Scaled();

	_float3 vRight =	_float3(1.f, 0.f, 0.f) * vScale.x;
	_float3 vUp =		_float3(0.f, 1.f, 0.f) * vScale.y;
	_float3 vLook =		_float3(0.f, 0.f, 1.f) * vScale.z;

	_float4x4 rotationMatrix{};
	
	D3DXMatrixRotationAxis(&rotationMatrix, &vAxis, m_fRotationPerSec);

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

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	_float3 vRight{ Get_State(STATE::RIGHT) }, vUp{ Get_State(STATE::UP) }, vLook{ Get_State(STATE::LOOK) };

	Set_State(STATE::RIGHT, *D3DXVec3Normalize(&vRight ,&vRight)* fScaleX);
	Set_State(STATE::UP, *D3DXVec3Normalize(&vUp, &vUp) * fScaleY);
	Set_State(STATE::LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fScaleZ);
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
