#include "Thirdee_Head.h"

CThirdee_Head::CThirdee_Head(LPDIRECT3DDEVICE9 pGraphic_Deivce)
	: CThirdee_Part{ pGraphic_Deivce }
{
}

CThirdee_Head::CThirdee_Head(const CThirdee_Head& Prototype)
	: CThirdee_Part{ Prototype }
{
}

HRESULT CThirdee_Head::Initialize_Prototype()
{

	return E_FAIL;
}

HRESULT CThirdee_Head::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize_Part(pArg);

	if (FAILED(Ready_Hair()))
		return E_FAIL;


	return S_OK;
}

void CThirdee_Head::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CThirdee_Head::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CThirdee_Head::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CThirdee_Head::Render()
{
	__super::Render();

	return S_OK;
}

void CThirdee_Head::AnimUpdate(_float fTimeDelta, PLAYERSTATE eState)
{
	m_fAnimTime += fTimeDelta * 5.f;

	m_fCurrentAnimRadian = (sinf(m_fAnimTime) * 0.2f);

	if (eState == PLAYERSTATE::MOVE)
	{
		_float4x4 TransformMatrix{};
		D3DXMatrixTranslation(&TransformMatrix, 0.f, 2.4f, m_fCurrentAnimRadian);

		m_LocalMatrix = m_LocalScaleMatrix * m_LocalRotationMatrix * TransformMatrix;
	}
	else if (eState == PLAYERSTATE::IDLE)
	{
		_float4x4 TransformMatrix{};
		D3DXMatrixTranslation(&TransformMatrix, m_fCurrentAnimRadian, 2.4f, 0.f);

		m_LocalMatrix = m_LocalScaleMatrix * m_LocalRotationMatrix * TransformMatrix;
	}
	
	for (auto& Child : m_Childs)
	{
		Child->AnimUpdate(fTimeDelta, eState);
	}
}

HRESULT CThirdee_Head::Ready_Hair()
{
	THIRDEEPART_DESC LeftHairDesc{};
	LeftHairDesc.LocalMatrix = m_LocalMatrix;
	LeftHairDesc.vLocalTransform = _float3(0.1f, -0.3f, 1.f);
	LeftHairDesc.vScale = _float3(0.3f, 0.7f, 0.3f);
	LeftHairDesc.vRotationRadian = _float3(D3DXToRadian(120.f), 0.f, 0.f);
	LeftHairDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Arm");
	LeftHairDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	LeftHairDesc.eType = PART_TYPE::HAIR;
	
	__super::Add_Child(LeftHairDesc);

	THIRDEEPART_DESC MidHairDesc{};
	MidHairDesc.LocalMatrix = m_LocalMatrix;
	MidHairDesc.vLocalTransform = _float3(0.1f, 1.f, 0.f);
	MidHairDesc.vScale = _float3(0.3f, 0.7f, 0.3f);
	MidHairDesc.vRotationRadian = _float3(0.f, 0.f, 0.f);
	MidHairDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Arm");
	MidHairDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	MidHairDesc.eType = PART_TYPE::HAIR;

	__super::Add_Child(MidHairDesc);

	THIRDEEPART_DESC RightHairDesc{};
	RightHairDesc.LocalMatrix = m_LocalMatrix;
	RightHairDesc.vLocalTransform = _float3(0.1f, -0.3f, -1.f);
	RightHairDesc.vScale = _float3(0.3f, 0.7f, 0.3f);
	RightHairDesc.vRotationRadian = _float3(D3DXToRadian(-120.f), 0.f, 0.f);
	RightHairDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Arm");
	RightHairDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
	RightHairDesc.eType = PART_TYPE::HAIR;

	__super::Add_Child(RightHairDesc);

	return S_OK;
}

CThirdee_Head* CThirdee_Head::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CThirdee_Head* pInstance = new CThirdee_Head(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created : CThirdee_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThirdee_Head::Clone(void* pArg)
{
	MSG_BOX(TEXT("CThirdee_Head Class is Can't Cloned"));

	return nullptr;
}

void CThirdee_Head::Free()
{
	__super::Free();
}
