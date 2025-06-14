#include "Thirdee_Body.h"

#include "Thirdee_Part.h"
#include "GameInstance.h"

CThirdee_Body::CThirdee_Body(LPDIRECT3DDEVICE9 pGraphic_Deivce)
	: CThirdee_Part{ pGraphic_Deivce }
{
}

CThirdee_Body::CThirdee_Body(const CThirdee_Body& Prototype)
	: CThirdee_Part{ Prototype }
{

}

HRESULT CThirdee_Body::Initialize_Prototype()
{
	
	return E_FAIL;
}

HRESULT CThirdee_Body::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	
	__super::Initialize_Part(pArg);

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	return S_OK;
}

void CThirdee_Body::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CThirdee_Body::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CThirdee_Body::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CThirdee_Body::Render()
{
	__super::Render();

	return S_OK;
}

void CThirdee_Body::AnimUpdate(_float fTimeDelta, PLAYERSTATE eState)
{
	for (auto& Child : m_Childs)
	{
		Child->AnimUpdate(fTimeDelta, eState);
	}

}

HRESULT CThirdee_Body::Ready_Parts()
{
	// Head
	THIRDEEPART_DESC HeadPartDesc{};
	HeadPartDesc.LocalMatrix = m_LocalMatrix;
	HeadPartDesc.vLocalTransform = _float3(0.f, 2.4f, 0.f);
	HeadPartDesc.vScale = _float3(1.7f, 1.5f, 1.2f);
	HeadPartDesc.vRotationRadian = _float3(0.f, 0.f, 0.f);
	HeadPartDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Head");
	HeadPartDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Sphere");
	HeadPartDesc.eType = PART_TYPE::HEAD;

	__super::Add_Child(HeadPartDesc);
	//Arm
	THIRDEEPART_DESC LeftArmDesc{};
	LeftArmDesc.LocalMatrix = m_LocalMatrix;
	LeftArmDesc.vLocalTransform = _float3(0.f, 0.5f, -0.7f);
	LeftArmDesc.vScale = _float3(0.5f, 1.f, 0.5f);
	LeftArmDesc.vRotationRadian = _float3(D3DXToRadian(45.f), 0.f, 0.f);
	LeftArmDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Arm");
	LeftArmDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	LeftArmDesc.eType = PART_TYPE::LEFT_ARM;

	__super::Add_Child(LeftArmDesc);

	THIRDEEPART_DESC RightArmDesc{};
	RightArmDesc.LocalMatrix = m_LocalMatrix;
	RightArmDesc.vLocalTransform = _float3(0.f, 0.5f, 0.7f);
	RightArmDesc.vScale = _float3(0.5f, 1.f, 0.5f);
	RightArmDesc.vRotationRadian = _float3(D3DXToRadian(-45.f), 0.f, 0.f);
	RightArmDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Arm");
	RightArmDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	RightArmDesc.eType = PART_TYPE::RIGHT_ARM;

	__super::Add_Child(RightArmDesc);

	//Regs
	THIRDEEPART_DESC LeftLegDesc{};
	LeftLegDesc.LocalMatrix = m_LocalMatrix;
	LeftLegDesc.vLocalTransform = _float3(0.f, -1.5f, 0.6f);
	LeftLegDesc.vScale = _float3(0.4f, 1.5f, 0.3f);
	LeftLegDesc.vRotationRadian = _float3(0.f, 0.f, 0.f);
	LeftLegDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Leg");
	LeftLegDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	LeftLegDesc.eType = PART_TYPE::LEFT_LEG;

	__super::Add_Child(LeftLegDesc);

	THIRDEEPART_DESC RightLegDesc{};
	RightLegDesc.LocalMatrix = m_LocalMatrix;
	RightLegDesc.vLocalTransform = _float3(0.f, -1.5f, -0.6f);
	RightLegDesc.vScale = _float3(0.4f, 1.5f, 0.3f);
	RightLegDesc.vRotationRadian = _float3(0.f, 0.f, 0.f);
	RightLegDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Leg");
	RightLegDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	RightLegDesc.eType = PART_TYPE::RIGHT_LEG;

	__super::Add_Child(RightLegDesc);

	return S_OK;
}


CThirdee_Body* CThirdee_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CThirdee_Body* pInstance = new CThirdee_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created : CThirdee_Body"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThirdee_Body::Clone(void* pArg)
{
	MSG_BOX(TEXT("CThirdee_Body Class is Can't Cloned"));

	return nullptr;
}

void CThirdee_Body::Free()
{
	__super::Free();
}
