#include "Block_Break.h"
#include "GameInstance.h"


CBlock_Break::CBlock_Break(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBlock{ pGraphic_Device }
{
}

CBlock_Break::CBlock_Break(const CBlock_Break& Prototype)
	: CBlock ( Prototype )
{
}

HRESULT CBlock_Break::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Break::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

	Ready_SubscribeEvent(m_iPlayLevel);

	__super::SetUp_BlockInfo(pArg);

	m_bIsStepOn = false;
	m_fBreakDelay = 2.f;
	m_fCurrentBreakTime = 0.f;
	m_fShakingPower = 0.25f;

	m_pColliderCom->ApplyFixedPosition(m_pTransformCom->Get_State(STATE::POSITION));

	name = TEXT("Block_Break");

    return S_OK;
}

void CBlock_Break::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Break::Update(_float fTimeDelta)
{

	if (m_bIsStepOn)
	{
		if (m_fCurrentBreakTime >= m_fBreakDelay && false == m_bDead)
		{
			m_pColliderCom->Collision_Off();

			m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
			m_pGameInstance->PlayAudio(TEXT("BlockBreak.wav"), CHANNELID::SOUND_EFFECT, 0.5f);
			m_bDead = true;
		}
		else
		{
			m_fCurrentBreakTime += fTimeDelta;
			Shaking();
		}
	}
}

void CBlock_Break::Late_Update(_float fTimeDelta)
{
	if(false == m_bDead)
		__super::Late_Update(fTimeDelta);
}

HRESULT CBlock_Break::Render()
{
	__super::Render();

	return S_OK;
}

void CBlock_Break::StepOn(const BLOCKBREAKEVENT& Event)
{
	if (false == m_bIsStepOn && IsNearBlock(Event.vPosition))
	{
		m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
		m_pGameInstance->PlayAudio(TEXT("BlockBreakShake.wav"), CHANNELID::SOUND_EFFECT, 0.5f);

		m_bIsStepOn = true;
		m_vCenterPosition = m_pTransformCom->Get_State(STATE::POSITION);
		BLOCKBREAKEVENT Event;
		Event.vPosition = m_vCenterPosition;
		
		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::BLOCK_BREAK, Event);
	}
}

_bool CBlock_Break::Compute_Near(const _float3& vOtherPosition)
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float3 vDistance = vPosition - vOtherPosition;

	_float fLength = D3DXVec3Length(&vDistance);

	return fLength <= 2.2f; //오차 범위 0.5f
}

_bool CBlock_Break::IsNearBlock(_float3 vPosition)
{
	_float3 fDistance = m_pTransformCom->Get_State(STATE::POSITION) - vPosition;
	fDistance.y = 0;

	return D3DXVec3Length(&fDistance) <= 2.2f;
}


void CBlock_Break::Shaking()
{
	
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	
	vPosition.x = m_vCenterPosition.x + (rand() % 50 / 100.f - m_fShakingPower);
	vPosition.z = m_vCenterPosition.z + (rand() % 50 / 100.f - m_fShakingPower);

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
}

HRESULT CBlock_Break::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Break"),
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
	ColliderDesc.vColliderScale = _float3(1.8f, 1.8f, 1.8f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlock_Break::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<BLOCKBREAKEVENT>(iPlayerLevel, EVENT_KEY::BLOCK_BREAK, [this](const BLOCKBREAKEVENT& Event) {
		this->StepOn(Event); });

	return S_OK;
}


void CBlock_Break::SetUp_RenderState()
{
	__super::SetUp_RenderState();

}

void CBlock_Break::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Break* CBlock_Break::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Break* pInstance = new CBlock_Break(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Break"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Break::Clone(void* pArg)
{
	CBlock_Break* pInstance = new CBlock_Break(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Break"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Break::Free()
{
	__super::Free();
}