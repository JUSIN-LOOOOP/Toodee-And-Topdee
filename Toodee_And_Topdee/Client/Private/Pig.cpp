#include "Pig.h"
#include "GameInstance.h"
#include "Parts.h"

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
	m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if(nullptr != pArg)
	{
		_float3* vPos = static_cast<_float3*>(pArg);
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

		m_pTransformCom->Set_State(STATE::POSITION, *vPos);
	}
	else
	{
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

		m_pTransformCom->Set_State(STATE::POSITION, _float3(0.5f, 0.f, 0.5f));
	}
	
	// m_pTransformCom->Scaling(3.f, 3.f, 3.f);

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	
	m_fMaxPat = 300.f;
	return S_OK;
}

void CPig::Priority_Update(_float fTimeDelta)
{

	int a = 10;
}

void CPig::Update(_float fTimeDelta)
{
	// if (m_pGameInstance->Key_Pressing(('X')))
	// {
	// 	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
	// 		m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);
	// 	else
	// 		m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);
	// }
	
	

	// if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
	// 	Move_Patrol(fTimeDelta);
	// 
	Test_KeyInput(fTimeDelta);

	for (auto& Pair : m_vParts)
	{
		if (nullptr != Pair.second)
			Pair.second->Update(m_pTransformCom, fTimeDelta, m_fWidth, m_fHeight);
	}
}

void CPig::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPig::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pTransformCom->Bind_Matrix();

	m_pVIBufferCom->Bind_Buffers();

	Render_Parts();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CPig::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
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
	// Phi 90 = 가로(객체기준 좌우)기준 중앙 / Theta 90 = 뒤쪽 (객체 기준) , -90 = 앞쪽 (객체 기준)
	CComponent* pComponent = { nullptr };

	CParts::PART_DESC PartDesc = {};
	// 공통 사항
	PartDesc.iTexLevelIndex = ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY);

	
	// 몸통 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Body");
	PartDesc.iTextureIndex = 0;
	PartDesc.vBodyScale = _float3(2.0f, 2.0f, 0.1f);
	PartDesc.fFrame = 1;
	PartDesc.fPhi = 90;
	PartDesc.iTextureMaxIndex = 9;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_PartBody"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Body"), static_cast<CParts*>(pComponent));
	PartDesc.fFrame = 0;




	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Eyes");
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fTheta = -75.f;
	PartDesc.fPhi = 45.f;
	// 왼쪽 눈 추가
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartLeftEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Left_Eye"), static_cast<CParts*>(pComponent));


	// 오른쪽 눈 추가
	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartRightEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Right_Eye"), static_cast<CParts*>(pComponent));

	// 코 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Nose");
	PartDesc.eState = CParts::PARTSTATE::PARTS_FRONT;
	PartDesc.fTheta = -90.f;
	PartDesc.fPhi = 90.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_PartNose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Nose"), static_cast<CParts*>(pComponent));

	// 꼬리 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Tail");
	PartDesc.eState = CParts::PARTSTATE::PARTS_BACK;
	PartDesc.fTheta = 90.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		TEXT("Com_PartTail"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Tail"), static_cast<CParts*>(pComponent));

	// 다리 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Legs");
	PartDesc.fTheta = 88.f;
	PartDesc.fPhi = 100.f;
	PartDesc.iTextureIndex = 0;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_PartLegs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Legs"), static_cast<CParts*>(pComponent));


	


	return S_OK;
}

void CPig::Render_Parts()
{
	
	if(m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
	{
		for (auto& Pair : m_vParts)
		{
			if (TEXT("Body") == Pair.first && nullptr != Pair.second)
			{
				_float3 vVec3 = _float3(0.f, 0.f, 0.f);
				Pair.second->Render(&vVec3);
			}
			return;
		}
	}
	else
	{
		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Render();
		}
	}


}

void CPig::Test_KeyInput(_float fTimeDelta)
{
	if (GetKeyState('W') < 0)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		m_fWidth = 0;
		m_fHeight = -10;
	}
	else if (GetKeyState('S') < 0)
	{
		m_pTransformCom->Go_Down(fTimeDelta);
		m_fWidth = 0;
		m_fHeight = 10;
	}
	else if (GetKeyState('A') < 0)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
		m_fWidth = -10;
		m_fHeight = 0;
	}
	else if (GetKeyState('D') < 0)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
		m_fWidth = 10;
		m_fHeight = 0;
	}
	else if (GetKeyState('E') < 0)
	{
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(10.f));
	}
	else
	{
		m_fWidth = 0;
		m_fHeight = 0;
	}
}

void CPig::Move_Patrol(_float fTimeDelta)
{
	m_pTransformCom->Go_Right(fTimeDelta);
	m_fPatrol += 100.f * fTimeDelta;
	if (m_fPatrol >= m_fMaxPat)
	{
		m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
		m_fPatrol = 0.f;
	}
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
}
