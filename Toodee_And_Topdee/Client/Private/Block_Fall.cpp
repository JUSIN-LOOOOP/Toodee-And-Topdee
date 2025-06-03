#include "Block_Fall.h"
#include "GameInstance.h"
#include "BlockState.h"

CBlock_Fall::CBlock_Fall(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CInteractionBlock{ pGraphic_Device }
{
}

CBlock_Fall::CBlock_Fall(const CBlock_Fall& Prototype)
    : CInteractionBlock{ Prototype }
{
}

HRESULT CBlock_Fall::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Fall::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Observer()))
		return E_FAIL;

	__super::Ready_State();

	__super::SetUp_BlockInfo(pArg);

	name = TEXT("Interaction_Block_Fall");

	m_fMaxFallHeight = 1.7f;
	m_fGravityPower = 10.f;
	m_fMaxGravityPower = 25.f;

    return S_OK;
}

void CBlock_Fall::Priority_Update(_float fTimeDelta)
{
	m_pGroundCheckColliderCom->Collision_Off();
	CheckDimension();
}

void CBlock_Fall::Update(_float fTimeDelta)
{
	if (m_bFallinHole)
		return;

	if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
	{
		if(m_bFall)
			Gravity(fTimeDelta);

		CheckCollisionToodeeState();
	}
	else
	{
		if (m_eCurrentState == BLOCKSTATE::STOP)
		{
			m_pTransformCom->Approach(ComputeTileCenter(), fTimeDelta, 10.f);
		}

		m_pCurrentState->Update(this, fTimeDelta);

		m_pGameInstance->Check_Collision(m_pColliderCom);

		__super::CheckCollisionTopdeeState();
	}
}

void CBlock_Fall::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (m_bFallinHole)
		return;

	if (m_eCurrentState == BLOCKSTATE::PUSH)
		__super::Update_PushState(this);

}

HRESULT CBlock_Fall::Render()
{
	__super::Render();

	return S_OK;
}

void CBlock_Fall::onReport(REPORT eReport, CSubjectObject* pSubject)
{
}

void CBlock_Fall::CheckDimension()
{
	if (m_ePrevDimension != m_pGameInstance->Get_CurrentDimension())
	{
		m_ePrevDimension = m_pGameInstance->Get_CurrentDimension();
		m_fGravityPower = 0.f;
	}

}

_float3 CBlock_Fall::ComputeTileCenter()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	const _float TILE_SIZE = 2.f;

	_int iCountX = static_cast<_int>(floorf(vPosition.x / TILE_SIZE));
	_int iCountZ = static_cast<_int>(floorf(vPosition.z / TILE_SIZE));

	_float fCenterX = {};
	_float fCenterZ = {};

	fCenterX = static_cast<_float>(iCountX) * TILE_SIZE + static_cast<_float>(TILE_SIZE * 0.5);
	fCenterZ = static_cast<_float>(iCountZ) * TILE_SIZE + static_cast<_float>(TILE_SIZE * 0.5);

	m_vCurrentTileCenter = { fCenterX, vPosition.y, fCenterZ };

	return m_vCurrentTileCenter;
}

void CBlock_Fall::CheckCollisionToodeeState()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	vPosition = { vPosition.x, vPosition.y, vPosition.z - m_fGroundCheckPosZ };

	m_pGroundCheckTransformCom->Set_State(STATE::POSITION, vPosition);

	m_pGroundCheckColliderCom->Collision_On();

	m_pGameInstance->Check_Collision(m_pGroundCheckColliderCom);

	if (m_pGroundCheckColliderCom->OnCollisionStay() || m_pGroundCheckColliderCom->OnCollisionEnter())
	{
		vector<CGameObject*>* pObjects = { nullptr };
		m_pGroundCheckColliderCom->GetOverlapAll(pObjects);

		if (pObjects)
		{
			for (auto iter : *pObjects)
			{
				if (iter->Get_Name().find(TEXT("Wall")) != string::npos || iter->Get_Name().find(TEXT("Block")) != string::npos)
				{
					m_bFall = false;
					m_fGravityPower = 0.f;
					m_pTransformCom->Set_State(STATE::POSITION, ComputeTileCenter());
				}

				if (iter->Get_Name().find(TEXT("Break")) != string::npos)
				{
					COLLIDER_DIR eBreakCollider_Dir = m_pGroundCheckColliderCom->DetectCollisionDirection();

					if (eBreakCollider_Dir == COLLIDER_DIR::BACK)
						Notify(EVENT::BLOCK_BREAK);
				}
			}
		}
	}
	else
		m_bFall = true;	
}

void CBlock_Fall::Gravity(_float fTimeDelta)
{
	if(m_fGravityPower < m_fMaxGravityPower)
		m_fGravityPower += GRAVITY * fTimeDelta;
	
	if (m_fGravityPower > m_fMaxGravityPower)
		m_fGravityPower = m_fMaxGravityPower;

	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	vPosition.z -= m_fGravityPower * fTimeDelta;

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
}

HRESULT CBlock_Fall::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Fall"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		GroundCheckTransformDesc{};
	GroundCheckTransformDesc.fSpeedPerSec = 0.f;
	GroundCheckTransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Ground_Check_Transform"), reinterpret_cast<CComponent**>(&m_pGroundCheckTransformCom), &GroundCheckTransformDesc)))
		return E_FAIL;

	/* ForCom_Collider */
	CCollider::COLLIDER_DESC		ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.vColliderScale = _float3(1.8f, 1.8f, 1.8f);
	ColliderDesc.bIsFixed = false;
	m_fGroundCheckPosZ = ColliderDesc.vColliderScale.z * 0.5f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC		GroundCheckColliderDesc{};
	GroundCheckColliderDesc.pOwner = this;
	GroundCheckColliderDesc.pTransform = m_pGroundCheckTransformCom;
	GroundCheckColliderDesc.vColliderPosion = m_pGroundCheckTransformCom->Get_State(STATE::POSITION);
	GroundCheckColliderDesc.vColliderScale = _float3(1.8f, 1.8f, 0.2f);
	GroundCheckColliderDesc.bIsFixed = false;
	m_fGroundCheckPosZ += GroundCheckColliderDesc.vColliderScale.z;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_GroundCheckCollider"), reinterpret_cast<CComponent**>(&m_pGroundCheckColliderCom), &GroundCheckColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBlock_Fall::Ready_Observer()
{
	m_pGameInstance->Subscribe_Observer(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Observer_BreakTrigger"), this);

	return S_OK;
}

void CBlock_Fall::SetUp_RenderState()
{
	__super::SetUp_RenderState();
}

void CBlock_Fall::Reset_RenderState()
{
	__super::Reset_RenderState();
}

CBlock_Fall* CBlock_Fall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBlock_Fall* pInstance = new CBlock_Fall(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBlock_Fall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlock_Fall::Clone(void* pArg)
{
	CBlock_Fall* pInstance = new CBlock_Fall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBlock_Fall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlock_Fall::Free()
{
	CInteractionBlock::Free();
	CSubjectObject::SubjectFree();

	Safe_Release(m_pGroundCheckColliderCom);
	Safe_Release(m_pGroundCheckTransformCom);


}
