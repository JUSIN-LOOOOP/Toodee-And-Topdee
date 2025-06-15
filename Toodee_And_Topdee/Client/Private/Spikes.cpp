#include "Spikes.h"
#include "GameInstance.h"

CSpikes::CSpikes(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject {pGraphic_Device }
{
}

CSpikes::CSpikes(const CSpikes& Prototype)
	: CGameObject { Prototype }
	, m_fAnimDelay { Prototype.m_fAnimDelay }
	, m_iMaxAnimCount {Prototype.m_iMaxAnimCount }
{
}

HRESULT CSpikes::Initialize_Prototype()
{
	name = TEXT("Enemy_Spikes");

	m_fAnimDelay = 0.08f;
	m_iMaxAnimCount = 5;

	return S_OK;
}

HRESULT CSpikes::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_eRenderDimension = m_pGameInstance->Get_CurrentDimension();
	
	switch (m_eRenderDimension)
	{
	case DIMENSION::TOODEE:
		m_iAnimCount = 0;
		break;
	case DIMENSION::TOPDEE:
		m_iAnimCount = 5;
		break;
	}

	m_bChange_Dimension = false;
	
	m_fCurrentAnimTime = 0.f;
	
	m_iAnimCount = 0;

	m_iCount = 0;

	
	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);
	_float3 vPos = pDesc->vPos;
	vPos.y = 1.f;
	m_pTransformCom->Set_State(STATE::POSITION, vPos);
	//m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	//m_pTransformCom->Go_Up(1.f);
	//m_pTransformCom->Scaling(2.f, 2.f, 2.f);

	if (m_pGameInstance->Get_NextLevelID() < ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1))
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	else
	{
		m_pTransformCom->TurnToRadian(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(90.f));
	}

	Ready_SubscribeEvent(m_pGameInstance->Get_NextLevelID());
	m_ParentMatrix = *m_pTransformCom->Get_WorldMatrix();
	m_ParentMatrix._43 -= 2.f;

	return S_OK;
}

void CSpikes::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_NextLevelID() < ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1))
		Check_Dimension();
}

void CSpikes::Update(_float fTimeDelta)
{
	if (GetAsyncKeyState('E'))
		m_Dead = true;
}

void CSpikes::Late_Update(_float fTimeDelta)
{
	if (m_bChange_Dimension)
		Update_AnimCount(fTimeDelta);

	if (m_pGameInstance->Get_CurrentLevelID() == ENUM_CLASS(LEVEL::LEVEL_STAGE1) ||
		m_pGameInstance->Get_CurrentLevelID() == ENUM_CLASS(LEVEL::LEVEL_STAGE4))
		RotationAround(m_ParentMatrix, 90.f, m_iAnimCount * 18.f);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CSpikes::Render()
{
	m_pColliderCom->Render();

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iAnimCount)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();
	


	return S_OK;
}


void CSpikes::Check_Dimension()
{
	if (m_eRenderDimension != m_pGameInstance->Get_CurrentDimension())
	{
		m_bChange_Dimension = true;
		
		switch (m_eRenderDimension)
		{
		case DIMENSION::TOODEE:
			m_iIncreaseAnimCount = 1;
			break;
		case DIMENSION::TOPDEE:
			m_iIncreaseAnimCount = -1;
			break;
		}

		m_eRenderDimension = m_pGameInstance->Get_CurrentDimension();
	}

}

void CSpikes::Update_AnimCount(_float fTimeDelta)
{
 	if (m_iCount >= 5)
	{
		m_bChange_Dimension = false;
		m_iCount = 0;
		return;
	}
	
	if (m_fCurrentAnimTime + fTimeDelta * 0.5f >= m_fAnimDelay)
	{
		m_iAnimCount += m_iIncreaseAnimCount;
		m_fCurrentAnimTime = 0.f;
		m_iCount++;
	}
	else
		m_fCurrentAnimTime += fTimeDelta * 0.5f;
}

void CSpikes::RotationAround(_float4x4 ParentMat, _float fRadianX, _float fRadianY)
{
	// 기준 위치에 대한 방향벡터들
	_float3 vRight		= static_cast<_float3>(ParentMat.m[0]);
	_float3 vUp			= static_cast<_float3>(ParentMat.m[1]);
	_float3 vLook		= static_cast<_float3>(ParentMat.m[2]);
	_float3 vWorldPos	= static_cast<_float3>(ParentMat.m[3]);

	_float fLengthX = D3DXVec3Length(&vRight);
	_float fLengthY = D3DXVec3Length(&vUp);
	_float fLengthZ = D3DXVec3Length(&vLook);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	_float fX{}, fY{}, fZ{};

	


	fX = fLengthX * sinf(D3DXToRadian(fRadianY)) * cosf(D3DXToRadian(-fRadianX));
	fY = fLengthY * cosf(D3DXToRadian(fRadianY));
	if(fRadianY == 90.f)
		fZ = 1.05f * sinf(D3DXToRadian(fRadianY)) * sinf(D3DXToRadian(-fRadianX));
	else
		fZ = fLengthZ * sinf(D3DXToRadian(fRadianY)) * sinf(D3DXToRadian(-fRadianX));
	
	_float3 vPos = vWorldPos + vRight * fX + vUp * fY + vLook * fZ;
	 
	

	m_pTransformCom->Set_State(STATE::POSITION, vPos);
}

HRESULT CSpikes::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Spikes"),
		TEXT("Com_Textrue"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpikes::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<REMOVE_SPIKE>(iPlayerLevel, EVENT_KEY::REMOVE_SPIKE, [this](const REMOVE_SPIKE& Event) {
		this->Remove(); });
	return S_OK;
}

HRESULT CSpikes::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSpikes::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CSpikes::Remove()
{
	m_Dead = true;
	_float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
	m_pGameInstance->Set_Active(TEXT("Effect_BlockDust"), &pos);
}

CSpikes* CSpikes::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpikes* pInstance = new CSpikes(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CSpikes "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpikes::Clone(void* pArg)
{
	CSpikes* pInstance = new CSpikes(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CSpikes "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpikes::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCom);
}
