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
	/*PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;
	m_strTexTag = pDesc->strTexTag;
	m_eState = pDesc->eState;
	m_vBodyScale = pDesc->vBodyScale;
	m_iTexLevelIndex = pDesc->iTexLevelIndex;

	if (pDesc->eState == PARTSTATE::PARTS_RIGHT)
	{
		m_fAngleX = -(pDesc->fAngleX);
		m_fAngleY = -(pDesc->fAngleY);
	}
	else
	{
		m_fAngleX = pDesc->fAngleX;
		m_fAngleY = pDesc->fAngleY;
	}*/

	return S_OK;
}

void CParts::Update(class CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
}


HRESULT CParts::Render(void* pArg)
{
	
	return S_OK;
}

void CParts::RevolveAround(class CTransform* pTransform, _int iAngleX, _int iAngleY, _float fRadius)
{

	_float3 vWorldPos = pTransform->Get_State(STATE::POSITION);
	_float3 vRight = pTransform->Get_State(STATE::RIGHT);
	_float3 vUp = pTransform->Get_State(STATE::UP);
	_float3 vLook = pTransform->Get_State(STATE::LOOK);

	// 객체의 방향벡터 받아와서 그대로적용 스케일값도 유지
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	
	_float fLengthX = D3DXVec3Length(&vRight);
	_float fLengthY = D3DXVec3Length(&vUp);

	_float fRadiusX = (fLengthX + fLengthX * fRadius) * 0.7f * 0.5f;
	_float fRadiusY = (fLengthY + fLengthY * fRadius) * 0.7f * 0.5f;

	// 각 방향 벡터들을 정규화
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	// 객체의 방향벡터를 이용하여 회전된 객체의 축을 중심으로 계산하게끔 설계
	_float fX{}, fY{}, fZ{};
	_float3 vPos = {};

	if (m_eState == PARTSTATE::PARTS_RIGHT)
	{
		fX = fRadiusX * sinf(D3DXToRadian(m_fAngleY - iAngleY)) * cosf(D3DXToRadian(m_fAngleX + iAngleX));
		fY = fRadiusY * cosf(D3DXToRadian(m_fAngleY - iAngleY));
		fZ = 0.2f * sinf(D3DXToRadian(m_fAngleY - iAngleY)) * sinf(D3DXToRadian(m_fAngleX + iAngleX));
	}
	else
	{
		fX = fRadiusX * sinf(D3DXToRadian(m_fAngleY + iAngleY)) * cosf(D3DXToRadian(m_fAngleX + iAngleX));
		fY = fRadiusY * cosf(D3DXToRadian(m_fAngleY + iAngleY));
		fZ = 0.2f * sinf(D3DXToRadian(m_fAngleY + iAngleY)) * sinf(D3DXToRadian(m_fAngleX + iAngleX));
	}

	vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;

	m_pTransformCom->Set_State(STATE::POSITION, vPos);

}

void CParts::Check_To_FocusDelta(_int* pOutX, _int* pOutY, _float3 vFocusPos, _float3 vMyPos)
{
	_float3 vDelta = vFocusPos - vMyPos;
	_int iX = *pOutX + static_cast<int>(vDelta.x);
	_int iY = *pOutY + static_cast<int>(vDelta.z);

	if (vDelta.x + vDelta.z > 0.1f) // 우측 상단삼각형
	{
		if (vDelta.x - vDelta.z < -0.1f) // 위쪽 삼각형 
		{
			if (*pOutX - static_cast<int>(vDelta.x) > 35)
				*pOutX = 35;
			else if (*pOutX - static_cast<int>(vDelta.x) < -35)
				*pOutX = -35;
			else
				*pOutX -= static_cast<int>(vDelta.x);


			*pOutY = 135;
		}
		else if (vDelta.x - vDelta.z > 0.1f) //우측 삼각형
		{
			if (iX > 90)
				*pOutX = 90;
			else if (iX < -90)
				*pOutX = -90;
			else
				*pOutX = iX;

			if (iY > 5)
				*pOutY = 5;
			else if (iY < -20)
				*pOutY = -20;
			else
				*pOutY = iY;
		}
	}
	else if (vDelta.x + vDelta.z < -0.1f) // 좌측 하단 삼각형
	{
		if (vDelta.x - vDelta.z > 0.1f) // 아래쪽 삼각형
		{
			if (iX > 45)
				*pOutX = 45;
			else if (iX < -45)
				*pOutX = -45;
			else
				*pOutX = iX;

			if (iY > 5)
				*pOutY = 5;
			else if (iY < -20)
				*pOutY = -20;
			else
				*pOutY = iY;
		}
		else if (vDelta.x - vDelta.z < -0.1f)  // 좌측 삼각형
		{
			if (iX > 90)
				*pOutX = 90;
			else if (iX < -90)
				*pOutX = -90;
			else
				*pOutX = iX;

			if (iY > 5)
				*pOutY = 5;
			else if (iY < -20)
				*pOutY = -20;
			else
				*pOutY = iY;
		}
	}
	else 
		return;
}

void CParts::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

}
