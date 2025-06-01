#include "Pig.h"
#include "GameInstance.h"
#include "Parts.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster {pGraphic_Device}
{
}

CPig::CPig(const CPig& Prototype)
	: CMonster {Prototype}
	, m_pTransformCom{ Prototype.m_pTransformCom }
	, m_pVIBufferCom{ Prototype.m_pVIBufferCom }
	, m_pColliderCom{ Prototype.m_pColliderCom }
	, m_bLeft{ Prototype.m_bLeft }
	, m_bMotion{ Prototype.m_bMotion }
{
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pVIBufferCom);
	Safe_AddRef(m_pColliderCom);
}

HRESULT CPig::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{
	name = TEXT("Pig");
	m_bLeft = false;

	m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	PIG_DESC* pDesc = static_cast<PIG_DESC*>(pArg);

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPosSet);

	

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	
	m_fMaxPat = 300.f;
	m_vScale = _float3(1.0f, 1.0f, 1.0f);
	
	return S_OK;
}

void CPig::Priority_Update(_float fTimeDelta)
{

	m_pGameInstance->Check_Collision(m_pColliderCom);
}

void CPig::Update(_float fTimeDelta)
{

	switch (m_pGameInstance->Get_CurrentDimension())
	{
	case::DIMENSION::TOODEE:
	
		if (!Check_Gravity(fTimeDelta))
			Move_Patrol(fTimeDelta);

		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, m_vFocusTargetPos);
		}
		break;

	case::DIMENSION::TOPDEE:

		if (m_bLeft) // TOPDEE로 전환시 혹시라도 TOODEE에서 방향이 뒤집혔다면 돌리는 작업
		{
			m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
			m_bLeft = false;
		}



		m_vFocusTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);

		m_pTransformCom->Move_To(m_vFocusTargetPos, fTimeDelta / 2.f);

		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, m_vFocusTargetPos);
		}
		break;
	}










	// if(GetKeyState('W') & 0x8000)
	// {
	// 	m_vScale *= 1.1f;
	// }
	// if (GetKeyState('S') & 0x8000)
	// {
	// 	m_vScale *= 0.9f;
	// 	
	// }
	// 
	// if (m_pColliderCom->OnCollisionEnter())
	// 	m_vScale *= 2.0f;
	// 
	// 
	// 	m_vScale *= 1.1 * fTimeDelta;
	// 
	// 
	// m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// Parts_Update(fTimeDelta);

}

void CPig::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPig::Render()
{
	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;

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

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 1.5f, 1.5f);
	// ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPig::Ready_Parts()
{
	CComponent* pComponent = { nullptr };
	CParts::PART_DESC PartDesc = {};

#pragma region Parts_Body

	PartDesc.iTexLevelIndex = ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY);			// 텍스쳐 컴포넌트 추가시 필요한 LevelIndex
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Body");		// 텍스쳐 컴포넌트 추가시 필요한 Tag
	PartDesc.iTextureIndex = 0;												// 텍스쳐 이미지 번호 (고정과 스프라이트 둘 다 하나의 텍스쳐 컴포넌트라면 고정이미지 0 그 이후 fFrame을 1로 설정)
	PartDesc.fFrame = 1.f;													// 텍스쳐 번호(스프라이트로 활용될 고정이미지 다음번호)
	PartDesc.iTextureMaxIndex = 9;											// 스프라이트(애니메이션)일 경우 마지막 이미지번호 fFrame <-> MaxIndex 순회하면서 이미지출력
	PartDesc.vBodyScale = _float3(3.0f, 3.0f, 0.1f);						// 이미지 기준 스케일 (객체의 콜리전박스와 다르게 지정하여 충돌체,이미지 구분가능)
	PartDesc.fAngleY = 1.f;													// 파츠들의 기본 배치 위치 (객체의 정중앙 = AngleX = 90 , AngleY = 90(후면), -90(전면)
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_PartBody"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Body"), static_cast<CParts*>(pComponent));

#pragma endregion

#pragma region Parts_Ear

	// Left_Ear
	PartDesc.fFrame = 0;
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Ears");
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleY = -70.f;
	PartDesc.fAngleX = 60.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_PartLeftEar"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Left_Ear"), static_cast<CParts*>(pComponent));


	// Right_Ear

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_PartRightEar"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Right_Ear"), static_cast<CParts*>(pComponent));

#pragma endregion

#pragma region Parts_Eye

	// Left_Eye
	PartDesc.fFrame = 0;
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Eyes");
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleY = -75.f;
	PartDesc.fAngleX = 45.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartLeftEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Left_Eye"), static_cast<CParts*>(pComponent));


	// Right_Eye

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartRightEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Right_Eye"), static_cast<CParts*>(pComponent));

#pragma endregion

#pragma region Parts_Nose

	// 코 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Nose");
	PartDesc.eState = CParts::PARTSTATE::PARTS_FRONT;
	PartDesc.fAngleY = -90.f;
	PartDesc.fAngleX = 90.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_PartNose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Nose"), static_cast<CParts*>(pComponent));

#pragma endregion

#pragma region Parts_Tail

	// 꼬리 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Tail");
	PartDesc.eState = CParts::PARTSTATE::PARTS_BACK;
	PartDesc.fAngleY = 90.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		TEXT("Com_PartTail"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Tail"), static_cast<CParts*>(pComponent));

#pragma endregion

#pragma region Parts_Legs

	// 다리 추가
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Legs");
	PartDesc.fAngleY = 88.f;
	PartDesc.fAngleX = 100.f;
	PartDesc.iTextureIndex = 0;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_PartLegs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Legs"), static_cast<CParts*>(pComponent));

#pragma endregion

	return S_OK;
}

void CPig::Render_Parts()
{
	
	if(m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
	{
		m_bMotion = true;
		for (auto& Pair : m_vParts)
		{
			if (TEXT("Body") == Pair.first && nullptr != Pair.second)
			{
				Pair.second->Render(&m_bMotion);
			}
			return;
		}
	}
	else
	{
		m_bMotion = false;
		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Render(&m_bMotion);
		}
	}


}


_bool CPig::Check_Gravity(_float fTimeDelta)
{
	_float fDelta = {};
	COLLIDER_DIR eDir = m_pColliderCom->DetectCollisionDirection(&fDelta);
	if (m_bGravity) // 중력있음
	{
		if (COLLIDER_DIR::CD_END == eDir && COLLIDER_DIR::BACK != eDir) // 충돌중이 아니거나 바닥에 충돌체가 없는경우 중력유지
		{
			_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);

			vPos.z -= GRAVITY * fTimeDelta;
			m_pTransformCom->Set_State(STATE::POSITION, vPos);
		}
		else //충돌중이면서 그 충돌이 하단인 경우 대해서만 처리 중력 꺼버림
		{
			m_bGravity = false;
			return false;
		}
		
		return true;
	}
	else // 중력 없음 TooDee <-> TopDee 전환시 이 값일 수 있으므로
	{
		if (COLLIDER_DIR::CD_END == eDir && COLLIDER_DIR::BACK != eDir) // 시점전환시 생길 문제에 대한 예외처리
		{
 			m_bGravity = true;
			return true;
		}

		return false;
	}	
}

void CPig::Move_Patrol(_float fTimeDelta)
{
	_float fX{};
	COLLIDER_DIR eDir = m_pColliderCom->DetectCollisionDirection(&fX);
	
	
	if(eDir == COLLIDER_DIR::RIGHT && fX > 0.1f)
	{
		m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
		m_bLeft = true;
	}
	else if (eDir == COLLIDER_DIR::LEFT && fX > 0.1f)
	{
		m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
		m_bLeft = false;
	}
	else if (m_pColliderCom->OnCollisionExit())
	{
		m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
		if (m_bLeft)
			m_bLeft = false;
		else
			m_bLeft = true;
	}

	m_pTransformCom->Go_Right(fTimeDelta);






	// m_pTransformCom->Go_Right(fTimeDelta);
	// m_fPatrol += 100.f * fTimeDelta;
	// if (m_fPatrol >= m_fMaxPat)
	// {
	// 	m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
	// 	m_fPatrol = 0.f;
	// 	if (m_bLeft)
	// 		m_bLeft = false;
	// 	else
	// 		m_bLeft = true;
	// 
	// }
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
	Safe_Release(m_pColliderCom);
}
