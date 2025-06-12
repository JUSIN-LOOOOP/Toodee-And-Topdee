#include "Thirdee_Leg.h"

CThirdee_Leg::CThirdee_Leg(LPDIRECT3DDEVICE9 pGraphic_Deivce)
	: CThirdee_Part{ pGraphic_Deivce }
{
}

CThirdee_Leg::CThirdee_Leg(const CThirdee_Leg& Prototype)
	: CThirdee_Part{ Prototype }
{
}

HRESULT CThirdee_Leg::Initialize_Prototype()
{

	return E_FAIL;
}

HRESULT CThirdee_Leg::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize_Part(pArg);

	m_fRadianPerSec = D3DXToRadian(45.f);
	m_fCurrentAnimRadian = 0.f;
	m_fAnimTime = 0.f;

	return S_OK;
}

void CThirdee_Leg::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CThirdee_Leg::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CThirdee_Leg::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CThirdee_Leg::Render()
{
	__super::Render();

	return S_OK;
}

void CThirdee_Leg::AnimUpdate(_float fTimeDelta, PLAYERSTATE eState)
{
	if (eState == PLAYERSTATE::MOVE)
	{
		m_fAnimTime += fTimeDelta * 5.f;

		m_fCurrentAnimRadian = (sinf(m_fAnimTime) * 0.3f) * m_fRadianPerSec;

		if (m_eType == PART_TYPE::RIGHT_LEG)
		{
			_float4x4 RotationMatrix{};
			D3DXMatrixRotationYawPitchRoll(&RotationMatrix, 0.f, 0.f, -m_fCurrentAnimRadian);
			m_LocalMatrix = m_LocalScaleMatrix * RotationMatrix * m_LocalTransformMatrix;
		}
		else if (m_eType == PART_TYPE::LEFT_LEG)
		{
			_float4x4 RotationMatrix{};
			D3DXMatrixRotationYawPitchRoll(&RotationMatrix, 0.f, 0.f, m_fCurrentAnimRadian);
			m_LocalMatrix = m_LocalScaleMatrix * RotationMatrix * m_LocalTransformMatrix;
		}
	}
	else
		m_LocalMatrix = m_LocalScaleMatrix * m_LocalRotationMatrix * m_LocalTransformMatrix;
}

CThirdee_Leg* CThirdee_Leg::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CThirdee_Leg* pInstance = new CThirdee_Leg(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created : CThirdee_Leg"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThirdee_Leg::Clone(void* pArg)
{
	MSG_BOX(TEXT("CThirdee_Leg Class is Can't Cloned"));

	return nullptr;
}

void CThirdee_Leg::Free()
{
	__super::Free();
}
