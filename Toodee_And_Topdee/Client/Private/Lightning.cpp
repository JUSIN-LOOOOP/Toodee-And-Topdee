#include "Lightning.h"
#include "GameInstance.h"
#include <cmath>

CLightning::CLightning(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPoolableObject{ pGraphic_Device }
{
}

CLightning::CLightning(const CLightning& Prototype)
	:CPoolableObject{ Prototype }
{
}

HRESULT CLightning::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLightning::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	//m_pTransformCom->Scaling(0.6f, 5.f, 0.4f);

	m_fMotionIntervalTime = { 0.08f };

	name = TEXT("Lightning");

	return S_OK;
}

void CLightning::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CLightning::Update(_float fTimeDelta)
{
	if (!m_bActive) return;
	Change_Motion(fTimeDelta);
	m_pGameInstance->Check_Collision(m_pColliderCom);

}

void CLightning::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CLightning::Render()
{
	if (!m_bActive) return S_OK;

	Bind_Lightning();

	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CLightning::Initialize_Pool(void* pArg)
{
	/* Pop되면 해당 멤버들 초기화 */
	LIGHTNING* info = reinterpret_cast<LIGHTNING*>(pArg);
	 m_vStartPosition = info->vStartPosition;
	m_vCrashPosition = info->vCrashPosition;

	m_fAccumulateMotionTime = 0.f;
	m_iMotionNum = 0;
	_float3 toCrashDirection = m_vCrashPosition - m_vStartPosition;
	_float fDist = D3DXVec3Length(&toCrashDirection);

	if(info->bSparkBlock)
		m_pColliderCom->Collision_Off();
	else
		m_pColliderCom->Collision_On();

	/* 너무 짧은 거리는 패스 */
	//if (fDist < 5.f)
	//	m_pGameInstance->Push(TEXT("Layer_Lightning"), this);

	/* 텍스쳐 중심 좌표들 모아놓는 벡터 클리어  */
	m_vecPositionCenters.clear();

	/* 내려치는 거리에 따른 텍스쳐 몇 장 필요한지 */
	if (fDist < 10.f)
		m_iNeedTextureCount = 1; 
	else
		m_iNeedTextureCount = static_cast<_uint>(ceil((fDist - 10.f) / 7.f));

	/* 번개 텍스처 간격 정하기 */
	m_fLightningIntervalDistance = fDist / static_cast<_float>(m_iNeedTextureCount);

	/* 텍스쳐들 중심 좌표 구하기*/
	D3DXVec3Normalize(&toCrashDirection, &toCrashDirection);
	for (_uint i = 1; i <= m_iNeedTextureCount; ++i)
	{
		_float3 centerPos = m_vStartPosition + toCrashDirection * (m_fLightningIntervalDistance * (static_cast<_float>(i-1) + 0.5f));
		m_vecPositionCenters.push_back(centerPos);
	}

	/* 트랜스폼 사이즈 조절. 콜라이더 사이즈 조절 *//* 위치 조정은 텍스처 찍으면서 조정해야함 */


	/* Toodee 일 때 회전 */
	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE) {
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
		m_pTransformCom->Scaling(1.f, m_fLightningIntervalDistance, 0.5f);
		m_pColliderCom->Set_Scaling(0.8f, 2.f, fDist * 0.55f);
	}
	else
	{
		m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(65.f));
		m_pTransformCom->Scaling(1.f, m_fLightningIntervalDistance, 0.5f);
		m_pColliderCom->Set_Scaling(0.8f, 12.f, fDist );

	}

	return S_OK;
}

HRESULT CLightning::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Lightning"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collision */
	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.f, 1.0f, 1.0f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		MSG_BOX(TEXT("Failed to Add_Component : Com_Collision"));
		return E_FAIL;
	}

	return S_OK;
}

void CLightning::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CLightning::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CLightning::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
	{
		m_fAccumulateMotionTime = 0.f;
		++m_iMotionNum;
		if (m_iMotionNum == 9)
		{
			m_pTransformCom->Set_State(STATE::POSITION, _float3(128.f, 128.f, 128.f));
			m_pColliderCom->Collision_Off();
			m_pGameInstance->Push(TEXT("Layer_Lightning"), this);
		}
	}
	else
		m_fAccumulateMotionTime += fTimeDelta;
}

HRESULT CLightning::Bind_Lightning()
{

	for (_float3 center : m_vecPositionCenters)
	{		
		if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
		{
			m_pTransformCom->Set_State(STATE::POSITION, _float3(128.f, 128.f, 128.f));
			m_pColliderCom->Collision_Off();
			m_pGameInstance->Push(TEXT("Layer_Lightning"), this);
			return S_OK;
		}

		SetUp_RenderState();
		m_pTransformCom->Set_State(STATE::POSITION, center);
		//m_pColliderCom->ApplyFixedPosition(center);

		m_pTransformCom->Bind_Matrix();

		if (FAILED(m_pTextureCom->Bind_Texture(m_iMotionNum)))
			return E_FAIL;

		m_pVIBufferCom->Bind_Buffers();

		m_pVIBufferCom->Render();
		Reset_RenderState();


	}


	return S_OK;
}

CLightning* CLightning::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLightning* pInstance = new CLightning(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLightning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLightning::Clone(void* pArg)
{
	CLightning* pInstance = new CLightning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLightning"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightning::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
