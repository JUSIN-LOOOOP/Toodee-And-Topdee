#include "Bat.h"
#include "GameInstance.h"
#include "Parts.h"
#include <Key.h>

CBat::CBat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster {pGraphic_Device}
{
}

CBat::CBat(const CBat& Prototype)
	: CMonster { Prototype }
{
}

HRESULT CBat::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CBat::Initialize(void* pArg)
{
	name = TEXT("Enemy_Monster_Bat");
	m_bLeft = false;
	m_eState = FLYSTATE::FLY_NON;
	m_fMaxDistance = 5.f * 5.f;
	m_iPlayLevel = m_pGameInstance->Get_CurrentLevelID();

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
	
	m_fOldHeight = pDesc->vPosSet.y;

	m_ePreDimension = m_pGameInstance->Get_CurrentDimension();
	if (m_ePreDimension == DIMENSION::TOODEE)
	{
		m_fCurAngle = m_fToodeeAngle; m_fCurLightY = m_fToodeeLightDirY;
	}
	else
	{
		m_fCurAngle = m_fTopdeeAngle; m_fCurLightY = m_fTopdeeLightDirY;
	}

	return S_OK;
}

void CBat::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Check_Collision(m_pColliderCom);
	
	ViewChange(fTimeDelta);
}

void CBat::Update(_float fTimeDelta)
{
	if(!m_bChangeView) // 카메라 전환중에는 진행하지않음
	{
		if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
		{

			Move_Patrol(fTimeDelta);
			for (auto& Pair : m_vParts)
			{
				if (nullptr != Pair.second)
					Pair.second->Update(m_pTransformCom, fTimeDelta, m_vToodeePos);
			}
		}
		else if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)
		{
			Move_To_Target(fTimeDelta);

			for (auto& Pair : m_vParts)
			{
				if (nullptr != Pair.second)
					Pair.second->Update(m_pTransformCom, fTimeDelta, m_pTargetTransformCom->Get_State(STATE::POSITION));
			}
		}
	}
	else
	{

		if (m_pColliderCom->OnCollisionEnter() || m_pColliderCom->OnCollisionStay())
			Move_Collision();


		for (auto& Pair : m_vParts)
		{
			if (nullptr != Pair.second)
				Pair.second->Update(m_pTransformCom, fTimeDelta);
		}

	}
}

void CBat::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBat::Render()
{
	Render_Shadow();

	m_pColliderCom->Render();

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

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_DiffuseCube"),
		TEXT("Com_VIBuffer2"), reinterpret_cast<CComponent**>(&m_VIBufferCom_Diffuse))))
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
	ColliderDesc.vColliderScale = _float3(3.0f, 3.0f, 3.0f);
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

	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Body")));
	PartDesc.fAngleY = 1.f;													// 파츠들의 기본 배치 위치 (객체의 정중앙 = AngleX = 90 , AngleY = 90(후면), -90(전면)
	PartDesc.fAngleX = 90.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Part_Body"),
		TEXT("Com_Bat_Body"), reinterpret_cast<CComponent**>(&pComponent), &PartDesc)))
		return E_FAIL;
	m_vParts.emplace(TEXT("Body"), pComponent);

#pragma endregion

#pragma region Parts_Ear

	// Left_Ear
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Ears")));
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
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Eyes")));
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
	PartDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Nose")));
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
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Legs")));
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
	pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STAGE4), TEXT("Prototype_Component_Texture_Bat_Wing")));
	PartDesc.pTextureCom = pTexture;
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
	
	// 
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
		if (Move_Collision())
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
		
		
		/*vector<CGameObject*>* findAll = {};
		m_pColliderCom->GetOverlapAll(findAll);
		for (auto& Other : *findAll)
		{
			_wstring strOtherName = Other->Get_Name();
			if (strOtherName.find(TEXT("Key")) != string::npos)
			{
				GETKEYEVENT Event;
				m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::GET_KEY, Event);
				CKey* pKey = dynamic_cast<CKey*>(Other);
				pKey->Get_Key();
				return;
			}
		}*/

		
	}

	m_vToodeePos.x += m_fMoveX;
}

_float CBat::MoveHeight(_float fStart, _float fEnd, _float fDistance)
{
	_float ft = fDistance * fDistance * (3.f - 2.f * fDistance);
	return _float((1 - ft) * fStart + ft * fEnd);
}

_bool CBat::Move_Collision()
{
	// 시점 변환시 충돌체 파악후 벽이면 밀려나게
	vector<CGameObject*>* findAll{};
	COLLIDER_SHAPE pDesc{};

	m_pColliderCom->GetOverlapAll(findAll);
	for (auto& Other : *findAll)
	{
		_wstring strOtherName = Other->Get_Name();		// 객체 이름으로 비교

		if (strOtherName.find(TEXT("Key")) != string::npos) // 열쇠일 경우 열쇠에 대한 event처리
		{
			GETKEYEVENT Event;
			m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::GET_KEY, Event);
			CKey* pKey = dynamic_cast<CKey*>(Other);
			pKey->Get_Key();
		}

		if (strOtherName.find(TEXT("Block")) != string::npos || strOtherName.find(TEXT("Wall")) != string::npos) // 객체 이름이 블럭이나 월이 들어가면 벽체
		{
			CCollider* OtherCollider = static_cast<CCollider*>(Other->Get_Component(TEXT("Com_Collider")));
			
			_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);		// 내 포지션값(콜라이더 포지션값)
			_float3 vRadius = m_pColliderCom->Get_ColliderScaled() * 0.5f;			// 내 콜라이더의 반지름
			_float3 vOtherPos = static_cast<CTransform*>(Other->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);		// 충돌체의 콜라이더 포지션값
			_float3 vOtherRadius = OtherCollider->Get_ColliderScaled() * 0.5f;		// 충돌체의 콜라이더 반지름
			_float3 vDistance = vOtherPos - vPos;							// 거리 구함
			_float3 vfinalRadius = vRadius + vOtherRadius;

			_float fOverlapX = vfinalRadius.x - fabsf(vDistance.x);
			_float fOverlapZ = vfinalRadius.z - fabsf(vDistance.z);

			if (0 <= fOverlapX && 0 <= fOverlapZ)
			{
				if (fOverlapX < fOverlapZ)
				{
					if (vDistance.x > 0.f) // 오른쪽
					{
						vPos.x -= fOverlapX+0.01f;
					}
					else
					{
						vPos.x += fOverlapX + 0.01f;
					}

				}
				else
				{
					if (vDistance.z > 0.f) // 위쪽
					{
						vPos.z -= fOverlapZ + 0.01f;
					}
					else
					{
						vPos.z += fOverlapZ + 0.01f;
					}
				}
			}
			m_pTransformCom->Set_State(STATE::POSITION, vPos);
			return true;
		}

	}
	
	return false;
}

void CBat::ViewChange(_float fTimeDelta)
{
	// change를 거치는 순간 bChagneView 를 true로 만듦
	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
	{	m_bLeft = false;	m_bChangeView = true;	}

	
	if (m_bChangeView)
	{
		
		_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
		if (m_pGameInstance->Get_PreviousDimension() == DIMENSION::TOODEE) // 이전 높이가 최저높이일 때  (Toodee -> Topdee로 전환됨)
		{
			vPos.y += m_fSpeedPerSec* fTimeDelta;

			if (vPos.y >= 5.f) // 최고높이 이상 도달하면 더 올라가지 않게 고정값 주고 false
			{	m_bChangeView = false;	vPos.y = 5.f;	m_fOldHeight = vPos.y;	}
		}
		else if (m_pGameInstance->Get_PreviousDimension() == DIMENSION::TOPDEE)  // 이전 높이가 최고높이일 때  (Topdee -> Toodee로 전환됨) 임의값 준거임
		{
			vPos.y -= m_fSpeedPerSec * fTimeDelta;

			if (vPos.y <= 0.5f) // 최저높이 이하 도달하면 더 내려가지 않게 고정값 주고 false
			{	m_bChangeView = false;	vPos.y = 0.5f;	m_fOldHeight = vPos.y;	}
		}
		
		m_pTransformCom->Set_State(STATE::POSITION, vPos);
	}
}


HRESULT CBat::Render_Shadow()
{
	Compute_AttributeShadow();

	_float3 vGroundPoint = _float3(0.f, -0.51f, 0.f);
	_float3 vGroundNormal = _float3(0.f, 1.f, 0.f);
	D3DXPLANE groundPlane;
	D3DXPlaneFromPointNormal(&groundPlane, &vGroundPoint, &vGroundNormal);

	_float angle = D3DXToRadian(m_fCurAngle);
	_float3 lightDir = _float3(cosf(angle), m_fCurLightY, sinf(angle));
	D3DXVec3Normalize(&lightDir, &lightDir);
	_float4 light = _float4(lightDir.x, lightDir.y, lightDir.z, 0.0f);

	_float4x4 matShadow;
	D3DXMatrixShadow(&matShadow, &light, &groundPlane);

	_float4x4 matWorld = *m_pTransformCom->Get_WorldMatrix();
	matWorld._11 = 2.5f;    matWorld._12 = matWorld._13 = 0.f;
	matWorld._22 = 1.f;     matWorld._21 = matWorld._23 = 0.f;
	matWorld._33 = 2.5f;    matWorld._31 = matWorld._32 = 0.f;
	m_matrixShadow = matShadow * matWorld;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matrixShadow);

	//텍스처 스테이지 상태 백업
	DWORD oldColorOp, oldColorArg1, oldAlphaOp, oldAlphaArg1;
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLOROP, &oldColorOp);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLORARG1, &oldColorArg1);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_ALPHAOP, &oldAlphaOp);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_ALPHAARG1, &oldAlphaArg1);

	//렌더 상태 백업
	DWORD oldAlphaEnable, oldSrcBlend, oldDestBlend, oldZWrite, oldLighting;
	m_pGraphic_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &oldAlphaEnable);
	m_pGraphic_Device->GetRenderState(D3DRS_SRCBLEND, &oldSrcBlend);
	m_pGraphic_Device->GetRenderState(D3DRS_DESTBLEND, &oldDestBlend);
	m_pGraphic_Device->GetRenderState(D3DRS_ZWRITEENABLE, &oldZWrite);
	m_pGraphic_Device->GetRenderState(D3DRS_LIGHTING, &oldLighting);

	//스텐실 버퍼
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILREF, 0x0);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);


	//그림자 렌더 설정
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<_uint>(110.f * m_fCurLightY), 8, 8, 8));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	// 그림자 렌더링
	m_VIBufferCom_Diffuse->Bind_Buffers();
	m_VIBufferCom_Diffuse->Render();

	//원복
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, oldColorArg1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, oldAlphaArg1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, oldAlphaEnable);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, oldSrcBlend);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, oldDestBlend);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, oldZWrite);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, oldLighting);
	return S_OK;
}

void CBat::Compute_AttributeShadow()
{
	if (!m_bCameraChange && m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
	{
		m_bCameraChange = true;
	}

	if (m_bCameraChange && !m_bComputeComplete) {
		if (m_ePreDimension == DIMENSION::TOPDEE)
		{
			if (m_fCurLightY <= m_fToodeeLightDirY)
				m_fCurLightY += 0.05f;

			if (m_fCurAngle >= m_fToodeeAngle - 360.f)
				m_fCurAngle -= 4.f;
			else
			{
				m_fCurLightY = m_fToodeeLightDirY;
				m_fCurAngle = m_fTopdeeAngle;
				m_bComputeComplete = true;
			}
		}
		else
		{
			if (m_fCurLightY <= m_fTopdeeLightDirY)
				m_fCurLightY -= 0.03f;

			if (m_fCurAngle >= m_fTopdeeAngle)
				m_fCurAngle -= 4.f;
			else
			{
				m_fCurLightY = m_fTopdeeLightDirY;
				m_fCurAngle = m_fTopdeeAngle;
				m_bComputeComplete = true;
			}

		}
	}
	if (m_bComputeComplete && m_pGameInstance->Get_CurrentDimension() != DIMENSION::CHANGE)
	{
		m_bComputeComplete = false;
		m_bCameraChange = false;
		m_ePreDimension = m_pGameInstance->Get_CurrentDimension();
		if (m_ePreDimension == DIMENSION::TOODEE)
		{
			m_fCurAngle = m_fToodeeAngle; m_fCurLightY = m_fToodeeLightDirY;
		}
		else
		{
			m_fCurAngle = m_fTopdeeAngle; m_fCurLightY = m_fTopdeeLightDirY;
		}
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
