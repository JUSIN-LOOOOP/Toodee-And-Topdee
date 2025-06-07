#include "Storm.h"
#include "GameInstance.h"
#include "PoolableObject.h"
#include "Lightning.h"
#include "Rain.h"
#include "MultiViewCamera.h"

CStorm::CStorm(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject { pGraphic_Device }
{
}

CStorm::CStorm(const CStorm& Prototype)
	:CGameObject{ Prototype }

{
}

HRESULT CStorm::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStorm::Initialize(void* pArg)
{
	/* Get Cloud - Transform */
	CGameObject* pGameObject = m_pGameInstance->Get_Ready_BackGameObject(TEXT("Layer_Cloud"));
	m_pCloudTransformCom = static_cast< CTransform* >(m_pGameInstance->Get_Ready_BackGameObject(TEXT("Layer_Cloud"))->Get_Component(TEXT("Com_Transform") ));
	if (m_pCloudTransformCom == nullptr) return E_FAIL;
	Safe_AddRef(m_pCloudTransformCom);

	/* Get Camera */
	m_pCamera = static_cast<CMultiViewCamera*>(m_pGameInstance->Get_Ready_BackGameObject(TEXT("Layer_Camera")));
	if (m_pCamera == nullptr) return E_FAIL;
	Safe_AddRef(m_pCamera);


	if (FAILED(Ready_Components()))
		return E_FAIL;

	name = TEXT("Storm");

	Collider_Position_Initailize();

	_float3 CloudPosition = m_pCloudTransformCom->Get_State(STATE::POSITION);
	for (_int i = -1; i <= 1; ++i)
	{
		m_Toodee_StartPosition[i + 1] = { CloudPosition.x + static_cast<_float>(i) * 1.3f ,CloudPosition.y , CloudPosition.z };
		m_Topdee_StartPosition[i + 1] = { CloudPosition.x + static_cast<_float>(i) * 1.3f - CLOUD_INTERVAL_POSITION_X ,CloudPosition.y + CLOUD_INTERVAL_POSITION_Y * 1.2f, CloudPosition.z - CLOUD_INTERVAL_POSITION_Y * 0.5f };
	}


	for (_uint i = 0; i < 3; ++i)
	{
		m_RainSpawnTime[i] = { 1.f, 0.f };
		m_LightningSpawnTime[i] = { 1.f,  0.f };
	}

	return S_OK;
}

void CStorm::Priority_Update(_float fTimeDelta)
{
}

void CStorm::Update(_float fTimeDelta)
{
	DIMENSION dimension = { m_pGameInstance->Get_CurrentDimension() };
	if (dimension != m_eDimension)
	{
		if (dimension != DIMENSION::CHANGE)
		{
			m_eDimension = dimension;
			if (m_eDimension == DIMENSION::TOODEE)
				m_bOnceToodee = true;
		}
	}
	for (_uint i = 0; i < 3; ++i)
		m_pTransformCom[i]->Bind_Matrix();

	for (_uint i = 0; i < 3; ++i)
		m_pGameInstance->Check_Collision(m_pColliderCom[i]);

	Compute_CrashSite(fTimeDelta);

	SpawnRain(fTimeDelta);

	SpawnRainSplash(fTimeDelta);

}

void CStorm::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CStorm::Render()
{
	for (_uint i = 0; i < 3; ++i)
		m_pColliderCom[i]->OBB_Render();

	return S_OK;
}

/* 항상 비 or 번개가 내려치므로 계속 계산.*/
void CStorm::Compute_CrashSite(_float fTimeDelta)
{
	for (_uint i = 0; i < 3; ++i)
	{
		vector<CGameObject*>* others;
		m_pColliderCom[i]->GetOverlapAll(others);

		_float3 myPos = m_pTransformCom[i]->Get_State(STATE::POSITION);
		_float fToodeeMinDist = FLT_MAX;
		_float fToodeeGroundMinDist = FLT_MAX;

		_float fTopdeeMinZ = { myPos.z + 3.f }, fMaxZ = { myPos.z + 4.f };
		_uint iCount = { 0 };

		for (CGameObject* other : *others)
		{
			CTransform* pOtherTransform = static_cast<CTransform*>(other->Get_Component(TEXT("Com_Transform")));
			if (pOtherTransform == nullptr) continue;

			_float3 otherPos = pOtherTransform->Get_State(STATE::POSITION);
			_float3 toOther = otherPos - myPos;
			_float fDistSq = D3DXVec3LengthSq(&toOther);

			if (m_eDimension == DIMENSION::TOODEE) {
				/* 구름보다 위에 있으면 */
				if (otherPos.z > myPos.z) continue;


				if (fDistSq < fToodeeGroundMinDist && other->CompareName(TEXT("Wall")))
				{
					fToodeeGroundMinDist = fDistSq;
					_float3 vLook = pOtherTransform->Get_State(STATE::LOOK);
					_float halfSizeZ = D3DXVec3Length(&vLook) * 0.5f;
					m_CrashGroundPosition[i] = { myPos.x, otherPos.y, otherPos.z + halfSizeZ };
				}

				if (fDistSq < fToodeeMinDist)
				{
					fToodeeMinDist = fDistSq;
					_float3 vLook = pOtherTransform->Get_State(STATE::LOOK);
					_float halfSizeZ = D3DXVec3Length(&vLook) * 0.5f;
					m_CrashAnyPosition[i] = { myPos.x, otherPos.y, otherPos.z + halfSizeZ };
				}
			}
			else if (m_eDimension == DIMENSION::TOPDEE)
			{
				if (otherPos.z < fTopdeeMinZ || otherPos.z > fMaxZ) ++iCount;
				else
				{
					if (fDistSq < fToodeeMinDist)
					{
						fToodeeMinDist = fDistSq;
						_float3 look = pOtherTransform->Get_State(STATE::LOOK);
						_float halfSizeZ = D3DXVec3Length(&look) * 0.5f;
						m_CrashAnyPosition[i] = { otherPos.x, otherPos.y, otherPos.z + halfSizeZ };
					}
				}

				if (iCount == others->size())
					m_CrashAnyPosition[i] = { myPos.x , 1.f , myPos.z - fMaxZ };
				m_CrashGroundPosition[i] = { myPos.x , 1.f , myPos.z - fMaxZ };;
			}
		}
	}
}

void CStorm::Collider_Position_Initailize()
{
	DIMENSION dimension = { m_pGameInstance->Get_CurrentDimension() };
	if (dimension != m_eDimension)
	{
		if (dimension != DIMENSION::CHANGE)
		{
			m_eDimension = dimension;
			if (m_eDimension == DIMENSION::TOODEE)
				m_bOnceToodee = true;
		}
	}
	for (_int i = -1; i <= 1; ++i)
	{
		m_pTransformCom[i + 1]->Set_WorldMatrix(*m_pCloudTransformCom->Get_WorldMatrix());

		_float3 vPosition = m_pTransformCom[i + 1]->Get_State(STATE::POSITION);

		m_pTransformCom[i + 1]->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(180.f));

		vPosition.x += static_cast<_float>(i) * 2.f;
		m_pTransformCom[i + 1]->Set_State(STATE::POSITION, vPosition);

		m_pColliderCom[i + 1]->ApplyFixedPosition(_float3(vPosition.x , vPosition.y, vPosition.z));
		m_pTransformCom[i + 1]->Bind_Matrix();
	}
}

void CStorm::SpawnRain(_float fTimeDelta)
{
	for (_uint i = 0; i < 3; ++i) 
	{
		if (m_RainSpawnTime[i].first + fTimeDelta >= m_RainSpawnTime[i].second)
		{
			m_RainSpawnTime[i] = { 0.f, m_pGameInstance->Rand(0.05f, 0.15f) };
			CPoolableObject* pRain = { nullptr };

			pRain = m_pGameInstance->Pop(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_Rain"));
			if (pRain == nullptr) {
				m_RainSpawnTime[i].second = m_pGameInstance->Rand(0.05f, 0.15f);
				continue;
			}
			CRain::RAIN info;

			if (m_eDimension == DIMENSION::TOODEE)
			{
				info.vStartPosition = m_Toodee_StartPosition[i];
			}
			else
			{
				info.vStartPosition = m_Topdee_StartPosition[i];
			}
			info.vStartPosition.x += m_pGameInstance->Rand(-1.f, 1.f);

			_float3 vCrashPosition = m_CrashGroundPosition[i];
			vCrashPosition.x += m_pGameInstance->Rand(-1.f, 1.f);
			info.vCrashPosition = vCrashPosition;

			pRain->Initialize_Pool(&info);
		}
		else
		{
			m_RainSpawnTime[i].first += fTimeDelta;
		}
	}

}

void CStorm::SpawnRainSplash(_float fTimeDelta)
{
	if (m_bOnceToodee)
	{
		if (m_fRainSplashIntervalTime <= m_fRainSplashAccumulateTime + fTimeDelta) {
			m_bOnceToodee = false;
			m_fRainSplashAccumulateTime = 0.f;
				for (_uint i = 0; i < 3; ++i) {
					CPoolableObject* pRainSplash = { nullptr };
					pRainSplash = m_pGameInstance->Pop(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Layer_RainSplash"));
					if (pRainSplash != nullptr)
					{
						_float3 pos = m_CrashGroundPosition[i];
						pos.y += 0.5f;
						pos.z += 1.0f;
						pRainSplash->Initialize_Pool(&pos);
					}
				}
		}
		else
			m_fRainSplashAccumulateTime += fTimeDelta;
	}

}

void CStorm::SpawnLightning(_float fTimeDelta)
{
	for (_uint i = 0; i < 3; ++i) {
		if (m_LightningSpawnTime[i].first + fTimeDelta >= m_LightningSpawnTime[i].second)
		{
			m_LightningSpawnTime[i] = { 0.f, m_pGameInstance->Rand(0.7f, 1.4f) };

			//스폰
		}
		else
		{
			m_LightningSpawnTime[i].first += fTimeDelta;
		}
	}

}

HRESULT CStorm::Ready_Components()
 {
	for (_uint i = 0; i < 3; ++i)
	{
		/* For.Com_Transform */
		CTransform::TRANSFORM_DESC		TransformDesc{};
		TransformDesc.fSpeedPerSec = 1.f;
		TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
			m_strArrayTransformTag[i], reinterpret_cast<CComponent**>(&m_pTransformCom[i]), &TransformDesc)))
			return E_FAIL;

		/* For.Com_Collision */
		CCollider::COLLIDER_DESC  ColliderDesc{};
		ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
		ColliderDesc.pTransform = m_pTransformCom[i];
		ColliderDesc.vColliderScale = _float3(1.f, 2.f, 68.f);
		ColliderDesc.bIsFixed = false;

		if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
			m_strArrayColliderTag[i], reinterpret_cast<CComponent**>(&m_pColliderCom[i]), &ColliderDesc)))
		{
			MSG_BOX(TEXT("Failed to Add_Component : Com_Collision"));
			return E_FAIL;
		}
	}


	return S_OK;
}


CStorm* CStorm::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStorm* pInstance = new CStorm(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CStorm"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStorm::Clone(void* pArg)
{
	CStorm* pInstance = new CStorm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CStorm"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStorm::Free()
{
	__super::Free();

	for (_uint i = 0; i < 3; ++i)
	{
		Safe_Release(m_pColliderCom[i]);
		Safe_Release(m_pTransformCom[i]);
	}

	Safe_Release(m_pCloudTransformCom);
	Safe_Release(m_pCamera);
	
}
