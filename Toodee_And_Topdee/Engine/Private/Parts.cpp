#include "Parts.h"
#include "GameInstance.h"

CParts::CParts(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{pGraphic_Device}
{
}

CParts::CParts(const CParts& Prototype)
	: CComponent{Prototype}
	, m_eState			{ Prototype.m_eState }
	, m_iTexLevelIndex	{Prototype.m_iTexLevelIndex }
	, m_strTexTag		{ Prototype.m_strTexTag }
	, m_pVIBufferCom	{ Prototype.m_pVIBufferCom }
	, m_pTransformCom	{ Prototype.m_pTransformCom }
	, m_pTextureCom		{ Prototype.m_pTextureCom  }
	, m_fTheta			{ Prototype.m_fTheta }
	, m_fPhi			{ Prototype.m_fPhi }

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
	m_iTexLevelIndex = pDesc->iTexLevelIndex;
	if (pDesc->eState == PARTSTATE::PS_RIGHT)
	{
		m_fTheta = -(pDesc->fTheta + pDesc->fParentRotation);
		m_fPhi = -(pDesc->fPhi + pDesc->fParentRotation);
	}
	else
	{
		m_fTheta = pDesc->fTheta + pDesc->fParentRotation;
		m_fPhi = pDesc->fPhi + pDesc->fParentRotation;
	}
	
	m_pVIBufferCom = static_cast<CVIBuffer_Rect*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_VIBuffer_Rect")));

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_Transform")));

	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, m_iTexLevelIndex, m_strTexTag));

	return S_OK;
}

void CParts::Update(class CTransform* pTransform, _float fTheta, _float fPhi)
{
}


HRESULT CParts::Render()
{
	
	return S_OK;
}

void CParts::RevolveAround(class CTransform* pTransform, _float fWidth, _float fDepth, _float fRadius)
{

	_float3 vWorldPos = pTransform->Get_State(STATE::POSITION);

	_float3 vRadiusX = pTransform->Get_State(STATE::RIGHT);
	_float3 vRadiusY = pTransform->Get_State(STATE::UP);
	_float3 vRadiusZ = pTransform->Get_State(STATE::LOOK);

	//// 위치 벡터
	//_float3 vWorldPos = ParentMatrix.m[3];
	//// x축 벡터(Right)
	//_float3 vRadiusX = ParentMatrix.m[0];
	//// y축 벡터(Up)
	//_float3 vRadiusY = ParentMatrix.m[1];
	//// z축 벡터(Look)
	//_float3 vRadiusZ = ParentMatrix.m[2];

	// 각 축에대한 반지름값 계산
	_float fRadiusX = D3DXVec3Length(&vRadiusX) / 2 + fRadius;
	_float fRadiusY = D3DXVec3Length(&vRadiusY) / 2 + fRadius;
	_float fRadiusZ = D3DXVec3Length(&vRadiusZ) / 2 + fRadius;

	_float3 vPos = {};


	if (m_eState == PARTSTATE::PS_RIGHT)
	{
		vPos.x = vWorldPos.x + fRadiusX * sinf(D3DXToRadian(m_fTheta)) * cosf(D3DXToRadian(m_fPhi + fWidth));
		vPos.y = vWorldPos.y + fRadiusY * cosf(D3DXToRadian(m_fTheta - fDepth));
		vPos.z = vWorldPos.z + sinf(D3DXToRadian(m_fTheta - fDepth)) * sinf(D3DXToRadian(m_fPhi + fWidth));
		//vPos.z = vWorldPos.z + fRadiusZ * sinf(D3DXToRadian(m_fTheta - fDepth)) * sinf(D3DXToRadian(m_fPhi + fWidth));

	}
	else
	{
		vPos.x = vWorldPos.x + fRadiusX * sinf(D3DXToRadian(m_fTheta)) * cosf(D3DXToRadian(m_fPhi + fWidth));
		vPos.y = vWorldPos.y + fRadiusY * cosf(D3DXToRadian(m_fTheta + fDepth));
		vPos.z = vWorldPos.z + sinf(D3DXToRadian(m_fTheta + fDepth)) * sinf(D3DXToRadian(m_fPhi + fWidth));
		//vPos.z = vWorldPos.z + fRadiusZ * sinf(D3DXToRadian(m_fTheta + fDepth)) * sinf(D3DXToRadian(m_fPhi + fWidth));
	}


	m_pTransformCom->Set_State(STATE::POSITION, vPos);
}

void CParts::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

}
