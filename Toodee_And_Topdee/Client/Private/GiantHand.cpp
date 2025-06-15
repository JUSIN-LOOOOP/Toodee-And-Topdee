#include "GiantHand.h"
#include "GameInstance.h"
#include "InteractionBlock.h"

CGiantHand::CGiantHand(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CGiantHand::CGiantHand(const CGiantHand& Prototype)
	: CGameObject{Prototype}
{
}

HRESULT CGiantHand::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGiantHand::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	_float3 vPosition = pDesc->vPos;

	vPosition.z = 1.f;
	vPosition.y = 22.f;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Scaling(50.f, 25.f, 1.f);
	m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->Set_State(STATE::POSITION, vPosition);

	_float3 ColliderPos = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderPos.y = 12.f;

	m_pColliderCom->ApplyFixedPosition(ColliderPos);

	m_bAction = false;

	name = TEXT("Hand");

	return S_OK;
}

void CGiantHand::Priority_Update(_float fTimeDelta)
{
}

void CGiantHand::Update(_float fTimeDelta)
{
	Check_Collision();

	if(m_bAction)
	{
		m_pTransformCom->Go_Down_UntilY(fTimeDelta, m_vTargetPosition.y);
		{
			ShakingZ(m_vOriginPosition.z, 0.25f);
		}
	}

}

void CGiantHand::Late_Update(_float fTimeDelta)
{
	if(m_bAction)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CGiantHand::Render()
{
	m_pColliderCom->Render();
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}



void CGiantHand::ShakingZ(_float fCenterPosZ, _float fShakingPower)
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	vPosition.z = fCenterPosZ + (rand() % 50 / 100.f - fShakingPower);

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
}

void CGiantHand::StartAction(_float UntilY)
{
	if(!m_bAction)
	{
		m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
		m_pGameInstance->PlayAudio(TEXT("GiantHand.wav"), CHANNELID::SOUND_EFFECT, 1.f);


		_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		vPosition.y += 10.f;
		m_pTransformCom->Set_State(STATE::POSITION, vPosition);
		vPosition.y = UntilY;

		m_vTargetPosition = vPosition;
		m_vOriginPosition = vPosition;

		m_pColliderCom->ApplyFixedPosition(m_vTargetPosition);
		name = TEXT("Wall");
	}
	m_bAction = true;

}

void CGiantHand::Check_Collision()
{
	m_pGameInstance->Check_Collision(m_pColliderCom);

	if (m_pColliderCom->OnCollisionEnter() || m_pColliderCom->OnCollisionStay())
	{
		vector<CGameObject*>* Overlaps;
		m_pColliderCom->GetOverlapAll(Overlaps);

		for (auto& iter : *Overlaps)
		{
			if (iter->Get_Name().find(TEXT("Player")) != std::wstring::npos)
				StartAction(12.5f);

			if (iter->Get_Name().find(TEXT("Metal")) != std::wstring::npos)
			{
				StartAction(14.5f);
				CInteractionBlock* pBlock = dynamic_cast<CInteractionBlock*>(iter);
				pBlock->Hold();
			}
		}
	}
}

HRESULT CGiantHand::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_FINALBOSS3), TEXT("Prototype_Component_Texture_GiantHand"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 60.f;
	TransformDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.5f, 25.f, 50.f);
	ColliderDesc.bIsFixed = false;
	
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CGiantHand::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CGiantHand::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CGiantHand* CGiantHand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGiantHand* pInstance = new CGiantHand(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : CGiantHand"));
	}

	return pInstance;
}

CGameObject* CGiantHand::Clone(void* pArg)
{
	CGiantHand* pInstance = new CGiantHand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : CGiantHand"));
	}

	return pInstance;
}

void CGiantHand::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
}
