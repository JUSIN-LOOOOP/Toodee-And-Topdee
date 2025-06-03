#include "Pig.h"
#include "GameInstance.h"
#include "Parts.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster {pGraphic_Device}
{
}

CPig::CPig(const CPig& Prototype)
	: CMonster { Prototype }
	// , m_vParts{ Prototype.m_vParts }
{
	//for (auto& Pair : m_vParts)
	//	Safe_AddRef(Pair.second);
}

HRESULT CPig::Initialize_Prototype()
{
	CParts* pComponent = { nullptr };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_PartBody"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Body"), pComponent);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_PartLeftEar"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Ear"), pComponent);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_PartRightEar"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Ear"), pComponent);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartLeftEye"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Eye"), pComponent);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartRightEye"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Eye"), pComponent);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_PartNose"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Nose"), pComponent);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		TEXT("Com_PartTail"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Tail"), pComponent);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_PartLegs"), reinterpret_cast<CComponent**>(&pComponent))))
		return E_FAIL;
	m_vParts.emplace(TEXT("Legs"), pComponent);

	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{
	name = TEXT("Monster_Pig");
	m_bLeft = false;

	m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	PIG_DESC* pDesc = static_cast<PIG_DESC*>(pArg);
	m_vOldPos = pDesc->vPosSet;
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	m_pTransformCom->Set_State(STATE::POSITION, m_vOldPos);

	

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	
	m_fMaxPat = 300.f;
	
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
	
		if (Check_Gravity(fTimeDelta))
			Compute_Collision();
		else
		{
			Move_Patrol(fTimeDelta);
		}
			

		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, m_pTargetTransformCom->Get_State(STATE::POSITION));
		}
		break;

	case::DIMENSION::TOPDEE:

		if (m_bLeft) // TOPDEE�� ��ȯ�� Ȥ�ö� TOODEE���� ������ �������ٸ� ������ �۾�
		{
			m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
			m_bLeft = false;
		}

		

		_float3 vMoveDir = Move_To_Target(fTimeDelta);
		// m_pTransformCom->Move_To(m_vFocusTargetPos, fTimeDelta* 0.5f);

		Compute_Collision(vMoveDir);

		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, m_pTargetTransformCom->Get_State(STATE::POSITION));
		}
		break;
	}




	if (GetKeyState('R') & 0x8000)
	{
		m_pTransformCom->Set_State(STATE::POSITION, m_vOldPos);
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Wall"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC	TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	m_vColliderScale = _float3(2.0f, 2.0f, 2.0f);
	m_vColliderRadius = m_vColliderScale * 0.5f;

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
	CParts::PART_DESC PartDesc = {};
	PartDesc.pVIBufferCom = m_pVIBufferCom;


#pragma region Parts_Body

	PartDesc.iTexLevelIndex = ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY);			// �ؽ��� ������Ʈ �߰��� �ʿ��� LevelIndex
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Body");		// �ؽ��� ������Ʈ �߰��� �ʿ��� Tag
	PartDesc.iTextureIndex = 0;												// �ؽ��� �̹��� ��ȣ (������ ��������Ʈ �� �� �ϳ��� �ؽ��� ������Ʈ��� �����̹��� 0 �� ���� fFrame�� 1�� ����)
	PartDesc.fFrame = 1.f;													// �ؽ��� ��ȣ(��������Ʈ�� Ȱ��� �����̹��� ������ȣ)
	PartDesc.iTextureMaxIndex = 9;											// ��������Ʈ(�ִϸ��̼�)�� ��� ������ �̹�����ȣ fFrame <-> MaxIndex ��ȸ�ϸ鼭 �̹������
	PartDesc.vBodyScale = _float3(3.0f, 3.0f, 1.f);						// �̹��� ���� ������ (��ü�� �ݸ����ڽ��� �ٸ��� �����Ͽ� �浹ü,�̹��� ���а���)
	PartDesc.fAngleY = 1.f;													// �������� �⺻ ��ġ ��ġ (��ü�� ���߾� = AngleX = 90 , AngleY = 90(�ĸ�), -90(����)
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_PartBody"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	
	m_vParts.emplace(TEXT("Body"), pComponent);
	// Safe_AddRef(pComponent);

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

	m_vParts.emplace(TEXT("Left_Ear"), pComponent);
	// Safe_AddRef(pComponent);

	// Right_Ear

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_PartRightEar"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Right_Ear"), pComponent);
	// Safe_AddRef(pComponent);

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

	m_vParts.emplace(TEXT("Left_Eye"), pComponent);
	// Safe_AddRef(pComponent);

	// Right_Eye

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_PartRightEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	 
	m_vParts.emplace(TEXT("Right_Eye"), pComponent);
	// Safe_AddRef(pComponent);

#pragma endregion

#pragma region Parts_Nose

	// �� �߰�
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Nose");
	PartDesc.eState = CParts::PARTSTATE::PARTS_FRONT;
	PartDesc.fAngleY = -90.f;
	PartDesc.fAngleX = 90.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_PartNose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Nose"), pComponent);
	// Safe_AddRef(pComponent);
#pragma endregion

#pragma region Parts_Tail

	// ���� �߰�
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Tail");
	PartDesc.eState = CParts::PARTSTATE::PARTS_BACK;
	PartDesc.fAngleY = 90.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
		TEXT("Com_PartTail"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Tail"), pComponent);
	// Safe_AddRef(pComponent);
#pragma endregion

#pragma region Parts_Legs

	// �ٸ� �߰�
	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Legs");
	PartDesc.fAngleY = 88.f;
	PartDesc.fAngleX = 100.f;
	PartDesc.iTextureIndex = 0;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_PartLegs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;

	m_vParts.emplace(TEXT("Legs"), pComponent);
	// Safe_AddRef(pComponent);
#pragma endregion
	//�ؿ��� �������

//#pragma region Parts_Body
//
//	PartDesc.iTexLevelIndex = ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY);			// �ؽ��� ������Ʈ �߰��� �ʿ��� LevelIndex
//	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Body");		// �ؽ��� ������Ʈ �߰��� �ʿ��� Tag
//	PartDesc.iTextureIndex = 0;												// �ؽ��� �̹��� ��ȣ (������ ��������Ʈ �� �� �ϳ��� �ؽ��� ������Ʈ��� �����̹��� 0 �� ���� fFrame�� 1�� ����)
//	PartDesc.fFrame = 1.f;													// �ؽ��� ��ȣ(��������Ʈ�� Ȱ��� �����̹��� ������ȣ)
//	PartDesc.iTextureMaxIndex = 9;											// ��������Ʈ(�ִϸ��̼�)�� ��� ������ �̹�����ȣ fFrame <-> MaxIndex ��ȸ�ϸ鼭 �̹������
//	PartDesc.vBodyScale = _float3(3.0f, 3.0f, 1.f);						// �̹��� ���� ������ (��ü�� �ݸ����ڽ��� �ٸ��� �����Ͽ� �浹ü,�̹��� ���а���)
//	PartDesc.fAngleY = 1.f;													// �������� �⺻ ��ġ ��ġ (��ü�� ���߾� = AngleX = 90 , AngleY = 90(�ĸ�), -90(����)
//	PartDesc.fAngleX = 90.f;
//
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
//		TEXT("Com_PartBody"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Body"), static_cast<CParts*>(pComponent));
//
//#pragma endregion
//
//#pragma region Parts_Ear
//
//	// Left_Ear
//	PartDesc.fFrame = 0;
//	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Ears");
//	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
//	PartDesc.fAngleY = -70.f;
//	PartDesc.fAngleX = 60.f;
//
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
//		TEXT("Com_PartLeftEar"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Left_Ear"), static_cast<CParts*>(pComponent));
//
//
//	// Right_Ear
//
//	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
//		TEXT("Com_PartRightEar"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Right_Ear"), static_cast<CParts*>(pComponent));
//
//#pragma endregion
//
//#pragma region Parts_Eye
//
//	// Left_Eye
//	PartDesc.fFrame = 0;
//	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Eyes");
//	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
//	PartDesc.fAngleY = -75.f;
//	PartDesc.fAngleX = 45.f;
//
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
//		TEXT("Com_PartLeftEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Left_Eye"), static_cast<CParts*>(pComponent));
//
//
//	// Right_Eye
//
//	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
//		TEXT("Com_PartRightEye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Right_Eye"), static_cast<CParts*>(pComponent));
//
//#pragma endregion
//
//#pragma region Parts_Nose
//
//	// �� �߰�
//	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Nose");
//	PartDesc.eState = CParts::PARTSTATE::PARTS_FRONT;
//	PartDesc.fAngleY = -90.f;
//	PartDesc.fAngleX = 90.f;
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
//		TEXT("Com_PartNose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Nose"), static_cast<CParts*>(pComponent));
//
//#pragma endregion
//
//#pragma region Parts_Tail
//
//	// ���� �߰�
//	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Tail");
//	PartDesc.eState = CParts::PARTSTATE::PARTS_BACK;
//	PartDesc.fAngleY = 90.f;
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Tail"),
//		TEXT("Com_PartTail"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Tail"), static_cast<CParts*>(pComponent));
//
//#pragma endregion
//
//#pragma region Parts_Legs
//
//	// �ٸ� �߰�
//	PartDesc.strTexTag = TEXT("Prototype_Component_Texture_Pig_Legs");
//	PartDesc.fAngleY = 88.f;
//	PartDesc.fAngleX = 100.f;
//	PartDesc.iTextureIndex = 0;
//	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
//		TEXT("Com_PartLegs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
//		return E_FAIL;
//
//	m_vParts.emplace(TEXT("Legs"), static_cast<CParts*>(pComponent));
//
//#pragma endregion

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


_bool CPig::Check_Gravity(_float fTimeDelta) // �ܼ� �߷� ������ ���� �˻��̹Ƿ� ������ �ʿ� ���� �����ؾ߰ڴ� (�̰� TOODEE��Ȳ�� ���� �Լ�)
{
	
	_float3 vMyPos = m_pTransformCom->Get_State(STATE::POSITION);
	vector<CGameObject*>* findAll = { nullptr };	// �浹ü ���θ� Ȯ���ϱ� ���� ����Ʈ�� ������������

	_float fDelta = {};
	COLLIDER_DIR eDir = m_pColliderCom->DetectCollisionDirection(&fDelta);
	m_pColliderCom->GetOverlapAll(findAll);

	// ������Ʈ �� �߷¿����� true, false�� ���� ������Ʈ�ÿ� ���� ó��
	if (m_bGravity)
	{
		if (m_pColliderCom->GetOverlapAll(findAll))
		{
			for (auto& Other : *findAll)
			{
				if (m_bGravity == false) // �浹ü ��ȸ���鼭 �ٴں��� �浹�Ͼ�� �߷� off
				{
					return false;
				}

				if (Other->Get_Name().find(TEXT("Block")) != string::npos) // �̸��� Block�� ���� �浹��ġ�� �ϴ��̴�
				{
					_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
					_float3 vDeltaPos = vOtherPos - vMyPos;
					_float3 vDeltaAbs = absfloat3(vDeltaPos);

					if(vDeltaAbs.x < vDeltaAbs.z && vDeltaPos.z < 0)
						m_bGravity = false;
				}
			}
		}
		else
		{
			vMyPos.z -= GRAVITY * fTimeDelta;
			m_pTransformCom->Set_State(STATE::POSITION, vMyPos);
			return true;
		}
	}
	else // ���� �߷� ���� TooDee <-> TopDee ��ȯ�� �� ���� �� �����Ƿ�
	{
		if(m_pColliderCom->GetOverlapAll(findAll)) // �浹ü ����
		{
			for (auto& Other : *findAll)
			{
				if (Other->Get_Name().find(TEXT("Block")) != string::npos) // �浹ü�� block�� �ƴϸ� �߷� ���� 
					continue;
		
			}
		}
		else // �浹ü�� ����
		{
			m_bGravity = true;
		}
	}	


	if (m_bGravity)
	{
		vMyPos.z -= GRAVITY * fTimeDelta;
		m_pTransformCom->Set_State(STATE::POSITION, vMyPos);
		return true;
	}
	else
		return false;

#pragma region ���� �� ����
	//	_float fDelta = {};
	//	COLLIDER_DIR eDir = m_pColliderCom->DetectCollisionDirection(&fDelta);
	//	if (m_bGravity) // �߷�����
	//	{
	//		if (COLLIDER_DIR::CD_END == eDir && COLLIDER_DIR::BACK != eDir) // �浹���� �ƴϰų� �ٴڿ� �浹ü�� ���°�� �߷�����
	//		{
	//			_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
	//	
	//			vPos.z -= GRAVITY * fTimeDelta;
	//			m_pTransformCom->Set_State(STATE::POSITION, vPos);
	//		}
	//		else //�浹���̸鼭 �� �浹�� �ϴ��� ��� ���ؼ��� ó�� �߷� ������
	//		{
	//			m_fDownBlock = fDelta;
	//			m_bGravity = false;
	//			return false;
	//		}
	//	
	//		return true;
	//	}
	//	else // �߷� ���� TooDee <-> TopDee ��ȯ�� �� ���� �� �����Ƿ�
	//	{
	//		if (COLLIDER_DIR::CD_END == eDir && COLLIDER_DIR::BACK != eDir) // ������ȯ�� ���� ������ ���� ����ó��
	//		{
	//			m_bGravity = true;
	//			return true;
	//		}
	//		return false;
	//	}
#pragma endregion
}

void CPig::Compute_Collision(_float3 vDir) // TopDee ����
{
	vector<CGameObject*>* findAll = { nullptr };
	if (!m_pColliderCom->GetOverlapAll(findAll)) // �̵� �� �浹 ������ �˻���������
		return;
	_float3 vMyPos{}, vMyRadius{}, vOldOtherPos{};
	_float fDeltaX{}, fDeltaZ{};
	_bool bPosX{}, bPosY{}, bPosZ{};

	vMyPos = m_pTransformCom->Get_State(STATE::POSITION);
	vMyRadius = m_pTransformCom->Get_Scaled() * 0.5f;

	for (auto& Other : *findAll)
	{
		_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
		_float3 vOtherRadius = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_Scaled() * 0.5f;
		_float3 vDeltaPos = vOtherPos - vMyPos;
		_float3 vDistance = (vMyRadius + vOtherRadius) - absfloat3(vDeltaPos);


		
		if (vDistance.x < vDistance.z && vOldOtherPos.z != vOtherPos.z) // �� �� 
		{
			// �浹ü�� ��ü���� ������ �ִ�
			if (vDeltaPos.x > 0 && vDir.x >= 0)		{ fDeltaX = -vDistance.x; }
			// �浹ü�� ��ü���� ������ �ִ�
			else if (vDeltaPos.x < 0 && vDir.x <= 0)		{ fDeltaX = vDistance.x; }
		}

		if (vDistance.x > vDistance.z && vOldOtherPos.x != vOtherPos.x) // �� ��
		{
			// �浹ü�� ��ü���� ���ʿ� �ִ�
			if (vDeltaPos.z > 0 && vDir.z >= 0)		{fDeltaZ = -vDistance.z;}
			// �浹ü�� ��ü���� �Ʒ��ʿ� �ִ�
			else if (vDeltaPos.z < 0 && vDir.z <= 0)		{fDeltaZ = vDistance.z;}
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
	

	m_pTransformCom->Go_Right(fTimeDelta); // �߷��� ���� �ϴ� �̵��Ѵ�.

	if (m_pColliderCom->GetOverlapAll(findAll))
	{
		if (1 == findAll->size()) // �浹��ü�� 1���̸� �ٴ� ��ü�̴� (�ٴڰ�ü�� 1:1�浹�ÿ��� �߷� off�� ������ �ٸ���Ȳ�� ����)
		{
			for (auto& Other : *findAll)
			{
				_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION); // �浹ü �����ǰ� �޾ƿ�
				_float fDelta = fabsf(vMyPos.x) - fabsf(vOtherPos.x);
				//if (m_pTransformCom->Get_Scaled().x * 0.5f < fDelta) // ��ü�� Collider ���������� ������� �ȶ������� �ϱ�����
				if (0.f < fDelta && m_pTransformCom->Get_Scaled().x * 0.5f > fDelta) // ��ü�� Collider ���������� ������� �ȶ������� �ϱ�����
				{
					if (vMyPos.x > vOtherPos.x) // ��ü x�� �浹ü�� x���� Ŭ��� ��ü(������), �浹ü(����) >>�̵��ϴٰ� ������������ 
					{
						m_bLeft = true;
						m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
						m_pTransformCom->Go_Right(fTimeDelta * 2.f);
					}
					else // ���� ���ǰ� �ݴ��� ��Ȳ (��üx�� �浹üx�� ������ ������ ��� x)
					{
						m_bLeft = false;
						m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
						m_pTransformCom->Go_Right(fTimeDelta * 2.f);
					}
				}
			}
			return;
		}
		else // �浹ü�� 2�� �̻� , �浹ü�� �ٴڸ� ���� ���� �ְ� �ٴڰ� ��(��)�ϼ��� ����.
		{
			for (auto& Other : *findAll)
			{
				_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
				if (Other->Get_Name().find(TEXT("Block")) != string::npos) // ��ü �̸��� block��
				{
					if (vMyPos.z > vOtherPos.z) // ���ε� ��ü���� �ؿ����� �����ϰ� ������ü��ȸ
						continue;
					else // �浹ü z���� ������ ���� (��,��) ���� ���� �����Ű���
					{
						if (vMyPos.x > vOtherPos.x) // ��ü x�� �浹ü�� x���� Ŭ��� ��ü(������), �浹ü(����) << �̵� �� �� ����ħ
						{
							m_bLeft = false;
							m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
							m_pTransformCom->Go_Right(fTimeDelta * 2.f);
						}
						else
						{
							m_bLeft = true;
							m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
							m_pTransformCom->Go_Right(fTimeDelta * 2.f);
						}
					}
				}
				else // ��ü �̸��� ���� �ƴѰ�쵵 �Դٰ��� �ؾ���
				{
				
					if (vMyPos.x > vOtherPos.x) // ��ü x�� �浹ü�� x���� Ŭ��� ��ü(������), �浹ü(����) << �̵� �� �� ����ħ
					{
						m_bLeft = false;
						m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
						m_pTransformCom->Go_Right(fTimeDelta * 2.f);
					}
					else
					{
						m_bLeft = true;
						m_pTransformCom->TurnToRadian(_float3(0.0f, 0.0f, 1.0f), D3DXToRadian(180.f));
						m_pTransformCom->Go_Right(fTimeDelta * 2.f);
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
		//Pair.second->Free();
		Safe_Release(Pair.second);
	}
	m_vParts.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
