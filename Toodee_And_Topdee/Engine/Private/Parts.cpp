#include "Parts.h"
#include "GameInstance.h"

CParts::CParts(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{pGraphic_Device}
{
}

CParts::CParts(const CParts& Prototype)
	: CComponent{Prototype}

{

}

HRESULT CParts::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParts::Initialize(void* pArg)
{
	
	return S_OK;
}

void CParts::Update(class CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
}

HRESULT CParts::Render()
{
	
	return S_OK;
}

void CParts::RevolveAround(class CTransform* pTransform, _float fAngleX, _float fAngleY, _float fRightLength, _float fUpLength)
{

	_float3 vWorldPos = pTransform->Get_State(STATE::POSITION);
	_float3 vRight = pTransform->Get_State(STATE::RIGHT);
	_float3 vUp = pTransform->Get_State(STATE::UP);
	_float3 vLook = pTransform->Get_State(STATE::LOOK);

	// ��ü�� ���⺤�� �޾ƿͼ� �״������ �����ϰ��� ����
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	
	_float fLengthX = D3DXVec3Length(&vRight);
	_float fLengthY = D3DXVec3Length(&vUp);

	_float fRadiusX = (fLengthX + (fLengthX * fRightLength)) * 0.8f * 0.5f;
	_float fRadiusY = (fLengthY + (fLengthY * fUpLength)) * 0.8f * 0.5f;

	// �� ���� ���͵��� ����ȭ
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	// ��ü�� ���⺤�͸� �̿��Ͽ� ȸ���� ��ü�� ���� �߽����� ����ϰԲ� ����
	_float fX{}, fY{}, fZ{};
	_float3 vPos = {};

	if (m_eState == PARTSTATE::PARTS_RIGHT)
	{
		fX = fRadiusX * sinf(D3DXToRadian(m_fAngleY - fAngleY)) * cosf(D3DXToRadian(m_fAngleX + fAngleX));
		fY = fRadiusY * cosf(D3DXToRadian(m_fAngleY - fAngleY));
		fZ = 0.2f * sinf(D3DXToRadian(m_fAngleY - fAngleY)) * sinf(D3DXToRadian(m_fAngleX + fAngleX));
	}
	else
	{
		fX = fRadiusX * sinf(D3DXToRadian(m_fAngleY + fAngleY)) * cosf(D3DXToRadian(m_fAngleX + fAngleX));
		fY = fRadiusY * cosf(D3DXToRadian(m_fAngleY + fAngleY));
		fZ = 0.2f * sinf(D3DXToRadian(m_fAngleY + fAngleY)) * sinf(D3DXToRadian(m_fAngleX + fAngleX));
	}

	vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;

	m_pTransformCom->Set_State(STATE::POSITION, vPos);

}

void CParts::Check_To_FocusDelta(_float* pOutX, _float* pOutY, _float3 vFocusPos, _float3 vMyPos)
{
	_float3 vDelta = vFocusPos - vMyPos;
	_float fX = *pOutX + vDelta.x;
	_float fY = *pOutY + vDelta.z;

	if (vDelta.x + vDelta.z > 0.1f) // ���� ��ܻﰢ��
	{
		if (vDelta.x - vDelta.z < -0.1f) // ���� �ﰢ�� 
		{
			if (static_cast<int>(fX) > 35)
				*pOutX = 35;
			else if (static_cast<int>(fX) < -35)
				*pOutX = -35;
			else
				*pOutX -= fX;


			*pOutY = 135;
		}
		else if (vDelta.x - vDelta.z > 0.1f) //���� �ﰢ��
		{
			if (static_cast<int>(fX) > 90)
				*pOutX = 90;
			else if (static_cast<int>(fX) < -90)
				*pOutX = -90;
			else
				*pOutX = fX;

			if (static_cast<int>(fY) > 5)
				*pOutY = 5;
			else if (static_cast<int>(fY) < -20)
				*pOutY = -20;
			else
				*pOutY = fY;
		}
	}
	else if (vDelta.x + vDelta.z < -0.1f) // ���� �ϴ� �ﰢ��
	{
		if (vDelta.x - vDelta.z > 0.1f) // �Ʒ��� �ﰢ��
		{
			if (static_cast<int>(fX) > 45)
				*pOutX = 45;
			else if (static_cast<int>(fX) < -45)
				*pOutX = -45;
			else
				*pOutX = fX;

			if (static_cast<int>(fY) > 5)
				*pOutY = 5;
			else if (static_cast<int>(fY) < -20)
				*pOutY = -20;
			else
				*pOutY = fY;
		}
		else if (vDelta.x - vDelta.z < -0.1f)  // ���� �ﰢ��
		{
			if (static_cast<int>(fX) > 90)
				*pOutX = 90;
			else if (static_cast<int>(fX) < -90)
				*pOutX = -90;
			else
				*pOutX = fX;

			if (static_cast<int>(fY) > 5)
				*pOutY = 5;
			else if (static_cast<int>(fY) < -20)
				*pOutY = -20;
			else
				*pOutY = fY;
		}
	}
	else 
		return;
}
void CParts::Look_At_degree(_float* pAngleXOut, _float* pAngleYOut, CTransform* pTransform, _float3 vTargetPos)
{

	_float3 vDeltaPos = vTargetPos - pTransform->Get_State(STATE::POSITION);
	_float3 vRight = pTransform->Get_State(STATE::RIGHT);
	_float3 vUp = pTransform->Get_State(STATE::UP);
	_float3 vLook = pTransform->Get_State(STATE::LOOK);

	_float3 vDeltaX = vDeltaPos;
	_float3 vUpX = vUp;
	_float3 vDeltaY = vDeltaPos;
	_float3 vRightY = vRight;
	_float3 vLookY = vLook;
	// z�� ���� (x���� ������� ����üũ)
	vDeltaX.y = 0.f; vUpX.y = 0.f;
	// x�� ���� (z���� ������� ����üũ)
	vDeltaY.y = 0.f; vRightY.y = 0.f;

	vUpX *= -1.f;

	D3DXVec3Normalize(&vDeltaX, &vDeltaX);
	D3DXVec3Normalize(&vDeltaY, &vDeltaY);
	D3DXVec3Normalize(&vUpX, &vUpX);
	D3DXVec3Normalize(&vRightY, &vRightY);


	_float fDotX = D3DXVec3Dot(&vUpX, &vDeltaX);
	_float fDegreeX = D3DXToDegree(acosf(fDotX));



	_float fDotY = D3DXVec3Dot(&vRightY, &vDeltaY);
	_float fDegreeY = D3DXToDegree(acosf(fDotY));

	_int a = 10;

	if (vDeltaPos.x < 0)	fDegreeX = -fDegreeX;
	if (vDeltaPos.z < 0)	fDegreeY = -fDegreeY;

	if (fDegreeY <= -20.f)	fDegreeY = -20.f; // �ϴܺι��⿡ ���� ��������(����)
	if (fDegreeY >= 45 )	fDegreeY = 30.f;

	*pAngleXOut = fDegreeX;
	*pAngleYOut = fDegreeY;
}

void CParts::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

}
