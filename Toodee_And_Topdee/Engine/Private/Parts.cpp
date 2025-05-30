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
	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);

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
	}


	m_pVIBufferCom = static_cast<CVIBuffer_Rect*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_VIBuffer_Rect")));

	// Safe_AddRef(m_pVIBufferCom);

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_Transform")));

	// Safe_AddRef(m_pTransformCom);

	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, m_iTexLevelIndex, m_strTexTag));

	// Safe_AddRef(m_pTextureCom);
	return S_OK;
}

void CParts::Update(class CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
}


HRESULT CParts::Render(void* pArg)
{
	
	return S_OK;
}

void CParts::RevolveAround(class CTransform* pTransform, _int iAngleX, _int iAngleY, _float fPartsScale)
{

	_float3 vWorldPos = pTransform->Get_State(STATE::POSITION);
	_float3 vRight = pTransform->Get_State(STATE::RIGHT);
	_float3 vUp = pTransform->Get_State(STATE::UP);
	_float3 vLook = pTransform->Get_State(STATE::LOOK);

	// �� ���� ���͵��� ����ȭ
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	
	// ��ü�� ���⺤�� �޾ƿͼ� �״������ (��ü�� ��ȸ�� �� ������ ���������� ȸ��) + ������ ���� ���Ͽ� �޾ƿ� ������ �� ����
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	// ��ü�� ���⺤�͸� �̿��Ͽ� ȸ���� ��ü�� ���� �߽����� ����ϰԲ� ����, ���⺤�� �����ص� �����ǰ�
	_float fX{}, fY{}, fZ{};
	_float3 vPos = {};
	_float3 vScale = m_vBodyScale * fPartsScale;

	if (m_eState == PARTSTATE::PARTS_RIGHT)
	{
		fX = (vScale.x / 2.f) * sinf(D3DXToRadian(m_fAngleY - iAngleY)) * cosf(D3DXToRadian(m_fAngleX + iAngleX));
		fY = (vScale.y / 2.f) * cosf(D3DXToRadian(m_fAngleY - iAngleY));
		fZ = (vScale.z / 2.f) * sinf(D3DXToRadian(m_fAngleY - iAngleY)) * sinf(D3DXToRadian(m_fAngleX + iAngleX) );

		vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;

	}
	else
	{
		fX = (vScale.x / 2.f) * sinf(D3DXToRadian(m_fAngleY + iAngleY)) * cosf(D3DXToRadian(m_fAngleX + iAngleX));
		fY = (vScale.y / 2.f) * cosf(D3DXToRadian(m_fAngleY + iAngleY));
		fZ = (vScale.z / 2.f) * sinf(D3DXToRadian(m_fAngleY + iAngleY)) * sinf(D3DXToRadian(m_fAngleX + iAngleX));

		vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;
	}
	

	m_pTransformCom->Scaling(vScale.x, vScale.y, vScale.z);
	m_pTransformCom->Set_State(STATE::POSITION, vPos);

}

void CParts::Check_To_FocusDelta(_int* pOutX, _int* pOutY, _float3 vFocusPos, _float3 vMyPos)
{
	_float3 vDelta = vFocusPos - vMyPos;
	_int iX = *pOutX + static_cast<int>(vDelta.x);
	_int iY = *pOutY + static_cast<int>(vDelta.z);

	if (vDelta.x + vDelta.z > 0.3f) // ���� ��ܻﰢ��
	{
		if (vDelta.x - vDelta.z < -0.3f) // ���� �ﰢ�� 
		{
			if (*pOutX - static_cast<int>(vDelta.x) > 45)
				*pOutX = 45;
			else if (*pOutX - static_cast<int>(vDelta.x) < -45)
				*pOutX = -45;
			else
				*pOutX -= static_cast<int>(vDelta.x);


			*pOutY = 135;
		}
		else if (vDelta.x - vDelta.z > 0.3f) //���� �ﰢ��
		{
			if (iX > 90)
				*pOutX = 90;
			else if (iX < -90)
				*pOutX = -90;
			else
				*pOutX = iX;

			if (iY > 20)
				*pOutY = 20;
			else if (iY < -20)
				*pOutY = -20;
			else
				*pOutY = iY;
		}
	}
	else if (vDelta.x + vDelta.z < -0.3f) // ���� �ϴ� �ﰢ��
	{
		if (vDelta.x + vDelta.z < -0.3) // �Ʒ��� �ﰢ��
		{
			if (iX > 45)
				*pOutX = 45;
			else if (iX < -45)
				*pOutX = -45;
			else
				*pOutX = iX;

			if (iY > 20)
				*pOutY = 20;
			else if (iY < -20)
				*pOutY = -20;
			else
				*pOutY = iY;
		}
		else if (vDelta.x + vDelta.z > 0.3)  // ���� �ﰢ��
		{
			if (iX > 90)
				*pOutX = 90;
			else if (iX < -90)
				*pOutX = -90;
			else
				*pOutX = iX;

			if (iY > 20)
				*pOutY = 20;
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
