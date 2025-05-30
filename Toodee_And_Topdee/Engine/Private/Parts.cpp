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
		m_fTheta = -(pDesc->fTheta);
		m_fPhi = -(pDesc->fPhi);
	}
	else
	{
		m_fTheta = pDesc->fTheta;
		m_fPhi = pDesc->fPhi;
	}


	m_pVIBufferCom = static_cast<CVIBuffer_Rect*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_VIBuffer_Rect")));

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_Transform")));

	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, m_iTexLevelIndex, m_strTexTag));

	
	return S_OK;
}

void CParts::Update(class CTransform* pTransform, _float fTimeDelta, _float InputWidth, _float InputDepth)
{
}


HRESULT CParts::Render(void* pArg)
{
	
	return S_OK;
}

void CParts::RevolveAround(class CTransform* pTransform, _float fWidth, _float fHeight, _float fPartsScale)
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
		fX = (vScale.x / 2.f) * sinf(D3DXToRadian(m_fTheta + fHeight)) * cosf(D3DXToRadian(m_fPhi + fWidth));
		fY = (vScale.y / 2.f)* cosf(D3DXToRadian(m_fTheta + fHeight));
		fZ = (vScale.z / 2.f)* sinf(D3DXToRadian(m_fTheta + fHeight)) * sinf(D3DXToRadian(m_fPhi + fWidth));

		vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;

	}
	else
	{
		fX = (vScale.x / 2.f) * sinf(D3DXToRadian(m_fTheta - fHeight)) * cosf(D3DXToRadian(m_fPhi + fWidth));
		fY = (vScale.y / 2.f) * cosf(D3DXToRadian(m_fTheta - fHeight));
		fZ = (vScale.z / 2.f) * sinf(D3DXToRadian(m_fTheta - fHeight)) * sinf(D3DXToRadian(m_fPhi + fWidth));

		vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;
	}
	

	m_pTransformCom->Scaling(vScale.x, vScale.y, vScale.z);
	m_pTransformCom->Set_State(STATE::POSITION, vPos);


}

void CParts::If_Revolved(_float& fWidth, _float& fHeight, _float fInputWidth, _float fInputHeight)
{

	if (0 != fInputHeight && 0 != fInputWidth && fInputHeight > fInputWidth)	//�밢�� �̵�
	{
		_float newHeight = fHeight + fInputHeight;
		_float newWidth = fWidth + fInputWidth;

		// ȸ�� ���� ���� üũ (-135 ~ 45)
		if (newHeight >= -135.0f && newHeight <= 45.0f)
		{
			fHeight = newHeight;
			fHeight += (fHeight < 0.0f ? 1 : -1) * fabsf(fInputWidth);
		}

		// �¿� ȸ�� ���� ���� üũ (-90 ~ 90)
		if (newWidth >= -90.0f && newWidth <= 90.0f)
		{
			fWidth = newWidth;
			fWidth += (fWidth < 0.0f ? 1 : -1) * fabsf(fInputHeight);
		}
		
	}

		// �Ѵ� 0�� �ƴѻ�Ȳ (������Ʈ�� Ű�Է��̶���� �̵����� ���� ���� 0�̾ƴѰ�� ex)��,��,��,�� �̵��� ���� ��)
	if (fInputHeight != 0.0f) // ��,�� �̵� �� ���� (TopView)
	{
		_float newHeight = fHeight + fInputHeight;

		// ȸ�� ���� ���� üũ (-135 ~ 45)
		if (newHeight >= -135.0f && newHeight <= 45.0f)
			fHeight = newHeight;

		// �¿찪 fWidth�� 0���� �ε巴�� �ǵ�����
		if (fabsf(fWidth) <= 10.0f)
		{
			fWidth = 0.0f;
		}
		else
		{
			fWidth += (fWidth < 0.0f ? 1 : -1) * fabsf(fInputHeight);
		}
		return;
	}

	if (fInputWidth != 0.0f) // �¿� �̵� �� ���� (SideView, TopView)
	{
		_float newWidth = fWidth + fInputWidth;

		// �¿� ȸ�� ���� ���� üũ (-90 ~ 90)
		if (newWidth >= -90.0f && newWidth <= 90.0f)
			fWidth = newWidth;

		// ���Ʒ��� fHeight�� 0���� �ε巴�� �ǵ�����
		if (fabsf(fHeight) <= 10.0f)
		{
			fHeight = 0.0f;
		}
		else
		{
			fHeight += (fHeight < 0.0f ? 1 : -1) * fabsf(fInputWidth);
		}
		return;
	}

	
}

void CParts::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

}
