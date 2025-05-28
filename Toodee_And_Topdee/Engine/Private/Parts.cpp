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

	// 각 방향 벡터들을 정규화
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	
	// 객체의 방향벡터 받아와서 그대로적용 (객체가 축회전 시 파츠도 같은축으로 회전) + 스케일 값을 곱하여 받아온 스케일 값 적용
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);

	// 객체의 방향벡터를 이용하여 회전된 객체의 축을 중심으로 계산하게끔 설계, 방향벡터 변경해도 유지되게
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

	if (0 != fInputHeight && 0 != fInputWidth && fInputHeight > fInputWidth)	//대각선 이동
	{
		_float newHeight = fHeight + fInputHeight;
		_float newWidth = fWidth + fInputWidth;

		// 회전 가능 범위 체크 (-135 ~ 45)
		if (newHeight >= -135.0f && newHeight <= 45.0f)
		{
			fHeight = newHeight;
			fHeight += (fHeight < 0.0f ? 1 : -1) * fabsf(fInputWidth);
		}

		// 좌우 회전 가능 범위 체크 (-90 ~ 90)
		if (newWidth >= -90.0f && newWidth <= 90.0f)
		{
			fWidth = newWidth;
			fWidth += (fWidth < 0.0f ? 1 : -1) * fabsf(fInputHeight);
		}
		
	}

		// 둘다 0이 아닌상황 (업데이트시 키입력이라던지 이동값에 대한 값이 0이아닌경우 ex)앞,뒤,좌,우 이동이 있을 시)
	if (fInputHeight != 0.0f) // 상,하 이동 값 있음 (TopView)
	{
		_float newHeight = fHeight + fInputHeight;

		// 회전 가능 범위 체크 (-135 ~ 45)
		if (newHeight >= -135.0f && newHeight <= 45.0f)
			fHeight = newHeight;

		// 좌우값 fWidth를 0으로 부드럽게 되돌리기
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

	if (fInputWidth != 0.0f) // 좌우 이동 값 있음 (SideView, TopView)
	{
		_float newWidth = fWidth + fInputWidth;

		// 좌우 회전 가능 범위 체크 (-90 ~ 90)
		if (newWidth >= -90.0f && newWidth <= 90.0f)
			fWidth = newWidth;

		// 위아래값 fHeight를 0으로 부드럽게 되돌리기
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
