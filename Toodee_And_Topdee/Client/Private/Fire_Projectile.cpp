#include "Fire_Projectile.h"
#include "GameInstance.h"

CFire_Projectile::CFire_Projectile(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPoolableObject{ pGraphic_Device }
{
}

CFire_Projectile::CFire_Projectile(const CFire_Projectile& Prototype)
    :CPoolableObject{ Prototype }

{
}

HRESULT CFire_Projectile::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFire_Projectile::Initialize(void* pArg)
{	
	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Set_State(STATE::POSITION, _float3(128.f, 128.f, 128.f));
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Scaling(2.f, 1.f, 2.f);
	name = TEXT("EnemyProjectile");

    return S_OK;
}

void CFire_Projectile::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) return;
}

void CFire_Projectile::Update(_float fTimeDelta)
{
	if (!m_bActive) return;

	Fire_Forward(fTimeDelta);
	Change_Motion(fTimeDelta);

	m_pGameInstance->Check_Collision(m_pColliderCom);

	if (m_pColliderCom->OnCollisionEnter()|| m_pColliderCom->OnCollisionStay())
	{		
		if (!m_pColliderCom->GetOverlapTarget()->CompareName(TEXT("Hole")))
		{
			m_pTransformCom->Set_State(STATE::POSITION, _float3(128.f, 128.f, 128.f));
			m_pGameInstance->Push(TEXT("Layer_Projectile_Fire"), this);
		}

	}

	if (m_fLifeInterval <= m_fAccumulateLifeTime + fTimeDelta)
	{
		m_pTransformCom->Set_State(STATE::POSITION, _float3(128.f, 128.f, 128.f));
		m_pGameInstance->Push(TEXT("Layer_Projectile_Fire"),this);
	}
	else
		m_fAccumulateLifeTime += fTimeDelta;
}

void CFire_Projectile::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) return;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CFire_Projectile::Render()
{
	if (!m_bActive) return S_OK;

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iMotionNum + static_cast<_uint>(m_eDir) * 10)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CFire_Projectile::Initialize_Pool(void* pArg)
{
    PROTECTILE* info = reinterpret_cast<PROTECTILE*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, info->vPosition);
	m_eDir = info->eDir;
	m_iMotionNum = { 0 };
	m_fAccumulateMotionTime = { 0.f };
	m_fAccumulateLifeTime = { 0.f };

	//m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	return S_OK;
}

HRESULT CFire_Projectile::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Projectile_Fire"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 1.0f, 1.0f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		MSG_BOX(TEXT("Failed to Add_Component : Com_Collider"));
		return E_FAIL;
	}

	return S_OK;
}

void CFire_Projectile::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CFire_Projectile::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CFire_Projectile::Fire_Forward(_float fTimeDelta)
{

	switch (m_eDir)
	{
	case Client::CCannon::CANNON_DIRECTION::RGIHT:
		m_pTransformCom->Go_Right(fTimeDelta);
		break;
	case Client::CCannon::CANNON_DIRECTION::LEFT:
		m_pTransformCom->Go_Left(fTimeDelta);
		break;
	case Client::CCannon::CANNON_DIRECTION::UP:
		m_pTransformCom->Go_Up(fTimeDelta);
		break;
	case Client::CCannon::CANNON_DIRECTION::DOWN:
		m_pTransformCom->Go_Down(fTimeDelta);
		break;;
	}
}

void CFire_Projectile::Change_Motion(_float fTimeDelta)
{
	if (m_fMotionIntervalTime <= m_fAccumulateMotionTime + fTimeDelta)
	{
		m_iMotionNum = ++m_iMotionNum == 10 ? 0 : m_iMotionNum;
	}
	else
		m_fAccumulateMotionTime += fTimeDelta;
}

CFire_Projectile* CFire_Projectile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFire_Projectile* pInstance = new CFire_Projectile(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CFire_Projectile"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFire_Projectile::Clone(void* pArg)
{
    CFire_Projectile* pInstance = new CFire_Projectile(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CFire_Projectile"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFire_Projectile::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

}
