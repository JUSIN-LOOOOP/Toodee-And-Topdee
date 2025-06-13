#include "Key.h"
#include "GameInstance.h"


CKey::CKey(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CKey::CKey(const CKey& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CKey::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CKey::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	m_pTransformCom->Go_Up(0.1f);
	m_pTransformCom->Scaling(2, 2, 2);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	name = TEXT("Key");

	m_pGameInstance->Add_Key();
	m_iTextureIdx = static_cast<_uint>(m_pGameInstance->Rand(0.f, 11.f));
	if (m_pGameInstance->Rand(0.f, 2.f) > 1.f)
		m_bTextureChangeDirection = true;

    return S_OK;
}

void CKey::Priority_Update(_float fTimeDelta)
{
}

void CKey::Update(_float fTimeDelta)
{
	Change_Motion(fTimeDelta);
}

void CKey::Late_Update(_float fTimeDelta)
{
	if(!IsDead())
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CKey::Render()
{
	m_pTransformCom->Bind_Matrix();
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIdx)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;
}

void CKey::Get_Key()
{
	m_Dead = true;
	m_pColliderCom->Collision_Off();

	m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
	m_pGameInstance->PlayAudio(TEXT("KeyPickup.wav"), CHANNELID::SOUND_EFFECT, 0.5f);
}

HRESULT CKey::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Key"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.vColliderScale = { 1.5f, 1.5f, 1.5f };
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_KeyCollider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

void CKey::Change_Motion(_float fTimeDelta)
{
	if (m_iIntervalTime <= m_iAccumulateTime + fTimeDelta)
	{
		m_iAccumulateTime = 0.f;
		if (m_bTextureChangeDirection)
		{
			++m_iTextureIdx;
			if (m_iTextureIdx == 12)
			{
				--m_iTextureIdx;
				m_bTextureChangeDirection = !m_bTextureChangeDirection;
			}
		}
		else
		{
			--m_iTextureIdx;
			if (m_iTextureIdx == -1)
			{
				++m_iTextureIdx;
				m_bTextureChangeDirection = !m_bTextureChangeDirection;
			}
		}
	
	}
	else
		m_iAccumulateTime += fTimeDelta;
}

CKey* CKey::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CKey* pInstance = new CKey(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKey::Clone(void* pArg)
{
	CKey* pInstance = new CKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKey::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
