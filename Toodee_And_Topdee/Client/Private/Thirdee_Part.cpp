#include "Thirdee_Part.h"
#include "GameInstance.h"
#include "Thirdee_Body.h"
#include "Thirdee_Head.h"
#include "Thirdee_Arm.h"
#include "Thirdee_Leg.h"
#include "Thirdee_Hair.h"


CThirdee_Part::CThirdee_Part(LPDIRECT3DDEVICE9 pGraphic_Deivce)
	: CGameObject { pGraphic_Deivce }
{
}

CThirdee_Part::CThirdee_Part(const CThirdee_Part& Prototype)
	: CGameObject { Prototype }
{
}

HRESULT CThirdee_Part::Initialize_Part(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	THIRDEEPART_DESC* pDesc = static_cast<THIRDEEPART_DESC*>(pArg);

	m_eType = pDesc->eType;

	D3DXMatrixIdentity(&m_LocalMatrix);

	_float4x4 ScaleMatrix{};
	D3DXMatrixScaling(&ScaleMatrix, pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);	
	
	
	_float4x4 RotationMatrix{};
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, pDesc->vRotationRadian.y, pDesc->vRotationRadian.x, pDesc->vRotationRadian.z);

	
	_float4x4 TransformMatrix{};
	D3DXMatrixTranslation(&TransformMatrix, pDesc->vLocalTransform.x, pDesc->vLocalTransform.y, pDesc->vLocalTransform.z);

	m_LocalScaleMatrix = ScaleMatrix;

	m_LocalRotationMatrix = RotationMatrix;

	m_LocalTransformMatrix = TransformMatrix;

	m_LocalMatrix = (ScaleMatrix * RotationMatrix * TransformMatrix);
	
	m_InitLocalMatrix = m_LocalMatrix;

	if (FAILED(Ready_Components(pDesc->strTextureTag, pDesc->strVIBufferTag)))
		return E_FAIL;

	
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

void CThirdee_Part::Priority_Update(_float fTimeDelta)
{
	for (auto& Child : m_Childs)
		Child->Priority_Update(fTimeDelta);
}

void CThirdee_Part::Update(_float fTimeDelta)
{

	m_WorldMatrix =  m_LocalMatrix * m_ParentWorldMatrix;
	m_pTransformCom->Set_WorldMatrix(m_WorldMatrix);

	for (auto& Child : m_Childs)
	{
		Child->Set_ParentWorldMatrix(m_WorldMatrix);
		Child->Update(fTimeDelta);
	}
}

void CThirdee_Part::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	for (auto& Child : m_Childs)
		Child->Late_Update(fTimeDelta);
}

HRESULT CThirdee_Part::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pVIBufferCom->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (auto& Child : m_Childs)
		Child->Render();

	return S_OK;
}


HRESULT CThirdee_Part::Ready_Components(_wstring& strTextureTag, _wstring& strVIBufferTag)
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), strVIBufferTag,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), strTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CThirdee_Part::Add_Child(THIRDEEPART_DESC PartDesc)
{

	CThirdee_Part* pInstance = { nullptr };

	switch (PartDesc.eType)
	{
	case PART_TYPE::HEAD:
		pInstance = CThirdee_Head::Create(m_pGraphic_Device, &PartDesc);
		break;

	case PART_TYPE::LEFT_ARM:
	case PART_TYPE::RIGHT_ARM:
		pInstance = CThirdee_Arm::Create(m_pGraphic_Device, &PartDesc);
		break;

	case PART_TYPE::LEFT_LEG:
	case PART_TYPE::RIGHT_LEG:
		pInstance = CThirdee_Leg::Create(m_pGraphic_Device, &PartDesc);
		break;
	case PART_TYPE::HAIR:
		pInstance = CThirdee_Hair::Create(m_pGraphic_Device, &PartDesc);
		break;
	}

	if (nullptr == pInstance)
	{
		MSG_BOX(TEXT("Failed Add Child"));
		return E_FAIL;
	}

	m_Childs.push_back(pInstance);

	return S_OK;
}

void CThirdee_Part::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	for (auto& Part : m_Childs)
		Safe_Release(Part);

	m_Childs.clear();

}
