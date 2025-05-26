#include "Pig.h"
#include "GameInstance.h"
#include "Part_Eyes.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject {pGraphic_Device}
{
}

CPig::CPig(const CPig& Prototype)
	: CGameObject {Prototype}
{
}


HRESULT CPig::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_pTransformCom->Set_State(STATE::POSITION, _float3(5.f, 5.f, 5.f));

	

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	
	return S_OK;
}

void CPig::Priority_Update(_float fTimeDelta)
{

	int a = 10;
}

void CPig::Update(_float fTimeDelta)
{
	// 테스트용 키입력
	if (GetKeyState('T') < 0)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_fTheta = 10;
		m_fPhi = 0;
	}

	else if (GetKeyState('G') < 0)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
		m_fTheta = -10;
		m_fPhi = 0;
	}

	else if (GetKeyState('F') < 0)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
		m_fTheta = 0;
		m_fPhi = -10;
	}

	else if (GetKeyState('H') < 0)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
		m_fTheta = 0;
		m_fPhi = 10;
	}
	else
	{
		m_fTheta = 0;
		m_fPhi = 0;
	}

	for (auto& Pair : m_vParts)
	{
		if (nullptr != Pair.second)
			Pair.second->Update(m_pTransformCom, m_fTheta, m_fPhi);
	}
	

}

void CPig::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPig::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	Render_Parts();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CPig::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Pig"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC	TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPig::Ready_Parts()
{

	CComponent* pComponent = { nullptr };

	CPart_Eyes::PART_DESC PartDesc = {};
	PartDesc.iTexLevelIndex = ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY);
	PartDesc.eState = PARTSTATE::PS_LEFT;
	PartDesc.fTheta = -60.f;
	PartDesc.fPhi = 60.f;
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig");
	PartDesc.iTextureIndex = 2;
	

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartLeftEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Left_Eye"), static_cast<CParts*>(pComponent));

	PartDesc.eState = PARTSTATE::PS_RIGHT;
	PartDesc.iTextureIndex = 3;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartRightEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Right_Eye"), static_cast<CParts*>(pComponent));


	PartDesc.eState = PARTSTATE::PS_FRONT;
	PartDesc.fTheta = -90.f;
	PartDesc.fPhi = 90.f;
	PartDesc.iTextureIndex = 4;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_PartNose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Nose"), static_cast<CParts*>(pComponent));
	

	PartDesc.eState = PARTSTATE::PS_BACK;
	PartDesc.fTheta = 90.f;
	PartDesc.iTextureIndex = 5;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		TEXT("Com_PartTail"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Tail"), static_cast<CParts*>(pComponent));





	


	return S_OK;
}

void CPig::Render_Parts()
{
	
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	for (auto& Pair : m_vParts)
	{
		if (nullptr != Pair.second)
			Pair.second->Render();
	}
	// 	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPig* pInstance = new CPig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPig"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

CGameObject* CPig::Clone(void* pArg)
{
	CPig* pInstance = new CPig(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPig"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

void CPig::Free()
{
	__super::Free();

	for (auto& Pair : m_vParts)
	{
		if (nullptr != Pair.second)
			Safe_Release(Pair.second);
	}
	m_vParts.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
