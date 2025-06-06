#include "Bat.h"
#include "GameInstance.h"
#include "Parts.h"

CBat::CBat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster {pGraphic_Device}
{
}

CBat::CBat(const CBat& Prototype)
	: CMonster { Prototype }
	// , m_vParts{ Prototype.m_vParts }
{
	//for (auto& Pair : m_vParts)
	//	Safe_AddRef(Pair.second);
}

HRESULT CBat::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CBat::Initialize(void* pArg)
{
	name = TEXT("Monster_Bat");
	m_bLeft = false;
	m_eState = FLYSTATE::FLY_NON;
	m_fMaxDistance = 5.f * 5.f;

	// m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	BAT_DESC* pDesc = static_cast<BAT_DESC*>(pArg);
	m_fSpeedPerSec = pDesc->fSpeedPerSec;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(4.f, 4.f, 4.f);
	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPosSet);

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	
	return S_OK;
}

void CBat::Priority_Update(_float fTimeDelta)
{

	m_pGameInstance->Check_Collision(m_pColliderCom);
	
}

void CBat::Update(_float fTimeDelta)
{

	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
	{
		_float3 vPos{};
		DIMENSION eDim = m_pGameInstance->Get_PreviousDimension();

		switch (eDim)
		{
		case::DIMENSION::TOODEE:
			vPos = m_pTransformCom->Get_State(STATE::POSITION);
		/*	if (vPos.y < 3.f)	vPos.y += m_fSpeedPerSec * fTimeDelta;

			if (vPos.y >= 3.f)	
			vPos.y = 3.f;*/
			vPos.y = 3.f;
			m_pTransformCom->Set_State(STATE::POSITION, vPos);
			m_bLeft = false;

			break;

		case::DIMENSION::TOPDEE:
			vPos = m_pTransformCom->Get_State(STATE::POSITION);
			/*if (vPos.y > 0.f)	vPos.y -= m_fSpeedPerSec * fTimeDelta;

			if (vPos.y <= 0.f)	*/
			
			vPos.y = 0.f;

			m_pTransformCom->Set_State(STATE::POSITION, vPos);
			m_bLeft = false;

			break;
		}
	}
	
	
	Key_Input(fTimeDelta);
	
	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
	{
		
		Move_Patrol(fTimeDelta);
		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, m_vToodeePos);
		}
	}
	else
	{
		if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)		Move_To_Target(fTimeDelta);
			
		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta, m_pTargetTransformCom->Get_State(STATE::POSITION));
		}
	}
		
}

void CBat::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBat::Render()
{
	//	if (FAILED(m_pColliderCom->Render()))
	//		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pTransformCom->Bind_Matrix();

	m_pVIBufferCom->Bind_Buffers();

	Render_Parts();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBat::Ready_Components()
{
	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	// /* For.Com_Texture */
	// if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Block_Wall"),
	// 	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	// 	return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC	TransformDesc{};
	TransformDesc.fSpeedPerSec = m_fSpeedPerSec;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(2.0f, 2.0f, 2.0f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBat::Ready_Parts()
{
	CParts* pComponent = { nullptr };
	CTexture* pTexture = { nullptr };
	CParts::PART_DESC PartDesc = {};
	PartDesc.pVIBufferCom = m_pVIBufferCom;
	PartDesc.strOtherName = name;


#pragma region Parts_Body

	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Bat_Body")));
	PartDesc.fAngleY = 1.f;													// 파츠들의 기본 배치 위치 (객체의 정중앙 = AngleX = 90 , AngleY = 90(후면), -90(전면)
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_Bat_Body"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Body"), pComponent);

#pragma endregion

#pragma region Parts_Ear

	// Left_Ear
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Bat_Ears")));
	PartDesc.pTextureCom = pTexture;
	PartDesc.fFrame = 0;
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleY = -45.f;
	PartDesc.fAngleX = 45.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_Bat_Left_Ear"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Ear"), pComponent);

	// Right_Ear
	Safe_AddRef(pTexture);

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Ears"),
		TEXT("Com_Bat_Right_Ear"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Ear"), pComponent);

#pragma endregion

#pragma region Parts_Eye

	// Left_Eye
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Bat_Eyes")));
	PartDesc.pTextureCom = pTexture;
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleY = -75.f;
	PartDesc.fAngleX = 40.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_Bat_Left_Eye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Eye"), pComponent);


	// Right_Eye
	Safe_AddRef(pTexture);

	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Eyes"),
		TEXT("Com_Bat_Right_Eye"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Eye"), pComponent);


#pragma endregion

#pragma region Parts_Nose

	// 코 추가
	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Bat_Nose")));
	PartDesc.eState = CParts::PARTSTATE::PARTS_FRONT;
	PartDesc.fAngleY = -100.f;
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Nose"),
		TEXT("Com_Bat_Nose"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Nose"), pComponent);

#pragma endregion

#pragma region Parts_Legs

	// 다리 추가
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Bat_Legs")));
	PartDesc.pTextureCom = pTexture;
	PartDesc.eState = CParts::PARTSTATE::PARTS_LEFT;
	PartDesc.fAngleX = 65.f;
	PartDesc.fAngleY = 145.f;
	


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_Bat_Left_Legs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Legs"), pComponent);

	Safe_AddRef(pTexture);
	PartDesc.eState = CParts::PARTSTATE::PARTS_RIGHT;
	PartDesc.fAngleX = -125.f;
	PartDesc.fAngleY = 145.f;
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Legs"),
		TEXT("Com_Bat_Right_Legs"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Legs"), pComponent);

#pragma endregion

#pragma region Parts_Wing

	// 날개 추가
	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Bat_Wing")));
	PartDesc.eState = CParts::PARTSTATE::PARTS_CENTER;
	PartDesc.fAngleX = 1.f;
	PartDesc.fAngleY = -90.f;
	PartDesc.fMaxFrame = 5;


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Wing"),
		TEXT("Com_Bat_Left_Wing"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Left_Wing"), pComponent);
	
	Safe_AddRef(pTexture);
	PartDesc.fAngleX = 0.f;
	PartDesc.fAngleY = 90.f;
	
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Wing"),
		TEXT("Com_Bat_Right_Wing"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Right_Wing"), pComponent);

#pragma endregion

	return S_OK;
}

void CBat::Render_Parts()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 170);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	for (auto& Pair : m_vParts)
	{
		if (nullptr != Pair.second)
			Pair.second->Render();
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBat::Move_To_Target(_float fTimeDelta)
{
	_float3 vMyPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	_float3 vDeltaPos = vTargetPos - vMyPos;
	_float fDistanceXZ = vDeltaPos.x * vDeltaPos.x + vDeltaPos.z * vDeltaPos.z;
	_float fPosY{}, fDistance{};
	
	if(fDistanceXZ > m_fMaxDistance)
		m_eState = FLYSTATE::FLY_NON;
	
	switch (m_eState)
	{
	case::CBat::FLYSTATE::FLY_UP:
		
		
		vMyPos.x += m_vTargetDir.x * m_fSpeedPerSec * fTimeDelta;
		vMyPos.z += m_vTargetDir.z * m_fSpeedPerSec * fTimeDelta;
		fDistance = clamp(fDistanceXZ / m_fMaxDistance, 0.f, 1.f);
		fPosY = MoveHeight(0.f, 3.f, fDistance);
		if (fDistanceXZ >= m_fMaxDistance)
		{
			fPosY = 3.f;
			m_eState = FLYSTATE::FLY_NON;
		}

		vMyPos.y = fPosY;
		m_pTransformCom->Set_State(STATE::POSITION, vMyPos);
		break;

	case::CBat::FLYSTATE::FLY_DOWN:

		vMyPos.x += m_vTargetDir.x * m_fSpeedPerSec * fTimeDelta;
		vMyPos.z += m_vTargetDir.z * m_fSpeedPerSec * fTimeDelta;
		fDistance = clamp(1.f - (fDistanceXZ / m_fMaxDistance), 0.f, 1.f);
		fPosY = MoveHeight(3.f, 0.f, fDistance);

		if (fDistanceXZ <= 0.1f)
		{
			fPosY = 0.f;
			m_eState = FLYSTATE::FLY_UP;
		}

		vMyPos.y = fPosY;

		m_pTransformCom->Set_State(STATE::POSITION, vMyPos);
		break;

	case::CBat::FLYSTATE::FLY_NON:
	

		if (m_pTransformCom->Move_To_LimitY(vTargetPos, fTimeDelta, m_fMaxDistance))
		{
			m_eState = FLYSTATE::FLY_DOWN;
			D3DXVec3Normalize(&m_vTargetDir, &vDeltaPos);
		}

		break;
	}


}

void CBat::Move_Patrol(_float fTimeDelta)
{
	m_vToodeePos = m_pTransformCom->Get_State(STATE::POSITION);
	m_vToodeePos.z -= 3.f;

	if (m_bLeft)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
		if(m_fMoveX < 3.f)	m_fMoveX += 0.5f;
	}
	else
	{
		m_pTransformCom->Go_Left(fTimeDelta);
		if(m_fMoveX > -3.f)	m_fMoveX -= 0.5f;
	}
	
	if (m_pColliderCom->OnCollisionEnter())
	{
		if (m_bLeft)
		{
			m_bLeft = false;
			m_pTransformCom->Go_Left(fTimeDelta * 2);
	
		}
		else
		{
			m_bLeft = true;
			m_pTransformCom->Go_Right(fTimeDelta * 2);
		}
	}

	m_vToodeePos.x += m_fMoveX;
}

_float CBat::MoveHeight(_float fStart, _float fEnd, _float fDistance)
{
	_float ft = fDistance * fDistance * (3.f - 2.f * fDistance);
	return _float((1 - ft) * fStart + ft * fEnd);
}

void CBat::Key_Input(_float fTimeDelta)
{
	
	if (GetKeyState('J') & 0x8000)
	{
		m_pTransformCom->TurnToRadian(_float3(0.f, 1.f, 0.f) , D3DXToRadian( 5.f));
	}
	if (GetKeyState('K') & 0x8000)
	{
		m_pTransformCom->TurnToRadian(_float3(1.f, 0.f, 0.f), D3DXToRadian(5.f));
	}
}

CBat* CBat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBat* pInstance = new CBat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBat"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

CGameObject* CBat::Clone(void* pArg)
{
	CBat* pInstance = new CBat(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBat"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

void CBat::Free()
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
