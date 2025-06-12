#include "Pig.h"
#include "GameInstance.h"
#include "Parts.h"
#include <Key.h>

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster {pGraphic_Device}
{
}

CPig::CPig(const CPig& Prototype)
	: CMonster { Prototype }
{

}

HRESULT CPig::Initialize_Prototype()
{
	CParts* pComponent = { nullptr };


	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{
	name = TEXT("Enemy_Monster_Pig");
	m_bLeft = false;
	m_iPlayLevel = m_pGameInstance->Get_CurrentLevelID();
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	PIG_DESC* pDesc = static_cast<PIG_DESC*>(pArg);
	m_vOldPos = pDesc->vPosSet;
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(4.f, 4.f, 4.f);
	m_pTransformCom->Set_State(STATE::POSITION, m_vOldPos);

	

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	
	m_fMaxPat = 300.f;
	
	return S_OK;
}

void CPig::Priority_Update(_float fTimeDelta)
{

	m_pGameInstance->Check_Collision(m_pColliderCom);
	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
	{
		if (m_bLeft) // TOPDEE로 전환시 혹시라도 TOODEE에서 방향이 뒤집혔다면 돌리는 작업
		{
			m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
			m_bLeft = false;
		}

	}
}

void CPig::Update(_float fTimeDelta)
{
	//	if (GetKeyState('O') & 0x8000)
	//		m_pColliderCom->Collision_On();
	//	if (GetKeyState('P') & 0x8000)
	//		m_pColliderCom->Collision_Off();
	//	
	//	_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
	//	if(vPos.z < -20.f)
	//	{
	//		vPos.z = 10.f; m_pTransformCom->Set_State(STATE::POSITION, vPos);
	//	}




	_float3 vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);

	switch (m_pGameInstance->Get_CurrentDimension())
	{
	case::DIMENSION::TOODEE:
	
		if (Check_Gravity(fTimeDelta))
			Compute_Collision();
		else
		{
			Move_Patrol(fTimeDelta);
		}
			

		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, vTargetPos);
		}
		break;

	case::DIMENSION::TOPDEE:

		_float3 vMoveDir = Move_To_Target(fTimeDelta);
		m_pTransformCom->Move_To(vTargetPos, fTimeDelta* 0.5f);

		Compute_Collision(vMoveDir);

		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, vTargetPos);
		}
		break;
	}
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

	/* For.Com_Texture */
	//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Wall"),
	//		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC	TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	m_vColliderScale = _float3(3.0f, 3.0f, 3.0f);

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = m_vColliderScale;
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPig::Ready_Parts()
{
	CParts* pComponent = { nullptr };
	CTexture* pTexture = { nullptr };
	CParts::PART_DESC PartDesc = {};
	PartDesc.pVIBufferCom = m_pVIBufferCom;
	PartDesc.strOtherName = name;


#pragma region Parts_Body

	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Body")));
	PartDesc.fFrame = 1.f;													// 텍스쳐 번호(스프라이트로 활용될 고정이미지 다음번호)
	PartDesc.fMaxFrame = 9.f;													// 스프라이트(애니메이션)일 경우 마지막 이미지번호 fFrame <-> MaxIndex 순회하면서 이미지출력
	PartDesc.fAngleY = 1.f;													// 파츠들의 기본 배치 위치 (객체의 정중앙 = AngleX = 90 , AngleY = 90(후면), -90(전면)
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_Pig_Body"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Body"), pComponent);

#pragma endregion

	// 고정이미지라 0으로 변경
	PartDesc.fFrame = 0.f;	
	PartDesc.fMaxFrame = 0.f;

#pragma region Parts_Ear
	// Left_Ear
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Ears")));
	PartDesc.pTextureCom = pTexture;
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleY = -70.f;
	PartDesc.fAngleX = 60.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_Pig_Left_Ear"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Ear"), pComponent);

	// Right_Ear
	Safe_AddRef(pTexture);

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_Pig_Right_Ear"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Ear"), pComponent);

#pragma endregion

#pragma region Parts_Eye
	// Left_Eye
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Eyes")));
	PartDesc.pTextureCom = pTexture;
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleY = -75.f;
	PartDesc.fAngleX = 45.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_Pig_Left_Eye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Eye"), pComponent);
	

	// Right_Eye
	Safe_AddRef(pTexture);

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_Pig_Right_Eye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Eye"), pComponent);
	

#pragma endregion

#pragma region Parts_Nose

	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Nose")));
	PartDesc.eState = CParts::PARTSTATE::PARTS_FRONT;
	PartDesc.fAngleY = -90.f;
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_Pig_Nose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Nose"), pComponent);

#pragma endregion

#pragma region Parts_Tail

	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Tail")));
	PartDesc.eState = CParts::PARTSTATE::PARTS_BACK;
	PartDesc.fAngleY = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		TEXT("Com_Pig_Tail"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Tail"), pComponent);

#pragma endregion

#pragma region Parts_Legs

	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Pig_Legs")));
	PartDesc.fAngleY = 88.f;
	PartDesc.fAngleX = 100.f;
	PartDesc.fMaxFrame = 7;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_Pig_Legs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Legs"), pComponent);

#pragma endregion





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
				Pair.second->Render();
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


_bool CPig::Check_Gravity(_float fTimeDelta) // Dimension이 TooDee일 경우만 들어오는 함수
{
	_float3 vMyPos = m_pTransformCom->Get_State(STATE::POSITION); // 내 위치값을 여러번 겟하는거보다 지역변수로 한번만 겟

	vector<CGameObject*>* findAll = { nullptr };	// 충돌체 여부를 확인하기 위한 리스트를 가져오기위함
	m_pColliderCom->GetOverlapAll(findAll);
	if (nullptr == findAll)
	{
		m_bGravity = true;
		vMyPos.z -= GRAVITY * fTimeDelta;
		m_pTransformCom->Set_State(STATE::POSITION, vMyPos);
		return true;
	}

	_float3 vOldOtherPos{}, vOldDistance{};

	for (auto& Other : *findAll)
	{

		if (Other->IsDead())
			return false;

		_wstring strOtherName = Other->Get_Name();
		if(strOtherName.find(TEXT("Key")) != string::npos)
		{
			GETKEYEVENT Event;
			m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::GET_KEY, Event);
			CKey* pKey = dynamic_cast<CKey*>(Other);
			pKey->Get_Key();
		}

		if (strOtherName.find(TEXT("Block")) != string::npos || strOtherName.find(TEXT("Wall")) != string::npos)
		{
			_float3 OtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
			_float fDistansZ = fabsf(OtherPos.z - vMyPos.z);

			if (OtherPos.z < vMyPos.z && fDistansZ > m_vColliderScale.z * 0.5f )// 충돌객체가 객체보다 밑에있다 (객체기준 8방향중 하단 3방향)
			{
				m_bGravity = false;
				return false;
			}
		}
		else
			continue;
	}

	m_bGravity = true;
	vMyPos.z -= GRAVITY * fTimeDelta;
	m_pTransformCom->Set_State(STATE::POSITION, vMyPos);
	return true;
}

void CPig::Compute_Collision(_float3 vDir)
{
	vector<CGameObject*>* findAll = { nullptr };
	if (!m_pColliderCom->GetOverlapAll(findAll)) // 이동 후 충돌 없으니 검사하지않음
		return;
	_float3 vMyPos{}, vMyRadius{}, vOldOtherPos{};
	_float fDeltaX{}, fDeltaZ{};
	_bool bPosX{}, bPosY{}, bPosZ{};

	vMyPos = m_pTransformCom->Get_State(STATE::POSITION);
	CCollider::COLLIDER_DESC pMyDesc = {};
	m_pColliderCom->Reference_Collider_Info(pMyDesc);
	vMyRadius = pMyDesc.vColliderScale * 0.5f;

	for (auto& Other : *findAll)
	{
		if (Other->IsDead())
			return;

		CCollider::COLLIDER_DESC pOtherDesc = {};
		static_cast<CCollider*>(Other->Get_Component(TEXT("Com_Collider")))->Reference_Collider_Info(pOtherDesc);
		_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
		_float3 vOtherRadius = pOtherDesc.vColliderScale * 0.5f;
		_float3 vDeltaPos = vOtherPos - vMyPos;
		_float3 vDistance = (vMyRadius + vOtherRadius) - absfloat3(vDeltaPos);



		if (vDistance.x < vDistance.z && vOldOtherPos.z != vOtherPos.z) // 좌 우 
		{
			// 충돌체가 객체보다 우측에 있다
			if (vDeltaPos.x > 0 && vDir.x >= 0) { fDeltaX = -vDistance.x; }
			// 충돌체가 객체보다 좌측에 있다
			else if (vDeltaPos.x < 0 && vDir.x <= 0) { fDeltaX = vDistance.x; }
		}

		if (vDistance.x > vDistance.z && vOldOtherPos.x != vOtherPos.x) // 상 하
		{
			// 충돌체가 객체보다 위쪽에 있다
			if (vDeltaPos.z > 0 && vDir.z >= 0) { fDeltaZ = -vDistance.z; }
			// 충돌체가 객체보다 아래쪽에 있다
			else if (vDeltaPos.z < 0 && vDir.z <= 0) { fDeltaZ = vDistance.z; }
		}

		vOldOtherPos = vOtherPos;
	}

	vMyPos.x += fDeltaX;
	vMyPos.z += fDeltaZ;

	m_pTransformCom->Set_State(STATE::POSITION, vMyPos);


}

void CPig::Move_Patrol(_float fTimeDelta)
{
	_float3 vMyPos = m_pTransformCom->Get_State(STATE::POSITION);
	vector<CGameObject*>* findAll = { nullptr };
	CCollider::COLLIDER_DESC pDesc;

	m_pTransformCom->Go_Right(fTimeDelta);


	if (m_pColliderCom->GetOverlapAll(findAll))
	{
		if (nullptr == findAll)
			return;

		if (1 == findAll->size()) // 충돌객체가 1개이며 바닥 객체이다 (바닥객체와 1:1충돌시에만 중력 off기 때문에 다른상황은 없음)
		{
			for (auto& Other : *findAll)
			{
				
				_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION); // 충돌체 포지션값 받아옴
				_float fDeltaX = (vMyPos.x - vOtherPos.x);
				static_cast<CCollider*>(Other->Get_Component(TEXT("Com_Collider")))->Reference_Collider_Info(pDesc);
				//if (m_pTransformCom->Get_Scaled().x * 0.5f < fDelta) // 객체의 Collider 반지름보다 벗어났을때 안떨어지게 하기위해
				if (fDeltaX > 0 && fDeltaX > (pDesc.vColliderScale.x * 0.5f)) // 양수, 객체보다 충돌체가 좌측
				{
					m_bLeft = false;
					m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
					m_pTransformCom->Go_Right(fTimeDelta * 2.f);
					return;
				}
				else if (fDeltaX < 0 && fDeltaX < (pDesc.vColliderScale.x * -0.5f)) // 음수, 객체보다 충돌체가 우측
				{
					m_bLeft = true;
					m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
					m_pTransformCom->Go_Right(fTimeDelta * 2.f);
					return;
				}
			}
			return;
		}
		else // 충돌체가 2개 이상
		{
			for (auto& Other : *findAll)
			{

				_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
				static_cast<CCollider*>(Other->Get_Component(TEXT("Com_Collider")))->Reference_Collider_Info(pDesc);
				
				
				_float fOtherMaxZ = vOtherPos.z + pDesc.vColliderScale.z * 0.5f; // 충돌 객체의 중심 z값 + z값에 대한 반지름 (충돌객체의 높이 판단) 벽이 아니라면 포지션값보다 높을수가 없음
				if(vMyPos.z <= fOtherMaxZ)
				{
					if (vMyPos.x > vOtherPos.x) // 객체 x가 충돌체의 x보다 클경우 객체(오른쪽), 충돌체(왼쪽) << 이동 중 벽 마주침
					{
						m_bLeft = false;
						m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
						m_pTransformCom->Go_Right(fTimeDelta * 2.f);
						return;
					}
					else
					{
						m_bLeft = true;
						m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
						m_pTransformCom->Go_Right(fTimeDelta * 2.f);
						return;
					}
				}
				
			}
		}
	}
				
			
}




_float3 CPig::Move_To_Target(_float fTimeDelta)
{
	_float3 vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	_float3 vMyPos = m_pTransformCom->Get_State(STATE::POSITION);

	_float3 vMoveDir = vTargetPos - vMyPos;

	// m_pTransformCom->Move_To(vTargetPos, fTimeDelta);
	
	return *D3DXVec3Normalize(&vMoveDir, &vMoveDir) ;
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
		Safe_Release(Pair.second);
	}
	m_vParts.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
