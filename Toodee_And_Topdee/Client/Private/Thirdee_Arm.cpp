#include "Thirdee_Arm.h"
#include "Transform.h"

CThirdee_Arm::CThirdee_Arm(LPDIRECT3DDEVICE9 pGraphic_Deivce)
    : CThirdee_Part{ pGraphic_Deivce }
{
}

CThirdee_Arm::CThirdee_Arm(const CThirdee_Arm& Prototype)
    : CThirdee_Part{ Prototype }
{
}

HRESULT CThirdee_Arm::Initialize_Prototype()
{

    return E_FAIL;
}

HRESULT CThirdee_Arm::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize_Part(pArg);

	m_fRadianPerSec = D3DXToRadian(45.f);
	m_fCurrentAnimRadian = 0.f;
	m_fAnimTime = 0.f;
	return S_OK;
}

void CThirdee_Arm::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CThirdee_Arm::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CThirdee_Arm::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CThirdee_Arm::Render()
{
    __super::Render();

    return S_OK;
}

void CThirdee_Arm::AnimUpdate(_float fTimeDelta, PLAYERSTATE eState)
{
	if (eState == PLAYERSTATE::MOVE)
	{
		 m_fAnimTime += fTimeDelta * 5.f;
		
		 m_fCurrentAnimRadian = (sinf(m_fAnimTime)* 0.3f) * m_fRadianPerSec;

		 if(m_eType == PART_TYPE::RIGHT_ARM)
		 {
			 _float4x4 RotationMatrix{};
			 D3DXMatrixRotationYawPitchRoll(&RotationMatrix, 0.f, D3DXToRadian(-45.f), m_fCurrentAnimRadian);
			 m_LocalMatrix = m_LocalScaleMatrix * RotationMatrix * m_LocalTransformMatrix;
		 }
		 else if(m_eType == PART_TYPE::LEFT_ARM)
		 {
			 _float4x4 RotationMatrix{};
			 D3DXMatrixRotationYawPitchRoll(&RotationMatrix, 0.f, D3DXToRadian(45.f), -m_fCurrentAnimRadian);
			 m_LocalMatrix = m_LocalScaleMatrix * RotationMatrix * m_LocalTransformMatrix;
		 }
	}
	else if (eState == PLAYERSTATE::ACTION)
	{
		m_fAnimTime += fTimeDelta * 5.f;

		m_fCurrentAnimRadian = (cosf(m_fAnimTime) * 0.4f) * m_fRadianPerSec;

		if (m_eType == PART_TYPE::RIGHT_ARM)
		{
			_float4x4 RotationMatrix{};
			D3DXMatrixRotationYawPitchRoll(&RotationMatrix, 0.f, D3DXToRadian(-150.f), 0.f);
			m_LocalMatrix = m_LocalScaleMatrix * RotationMatrix * m_LocalTransformMatrix;
		}
		else if (m_eType == PART_TYPE::LEFT_ARM)
		{
			_float4x4 RotationMatrix{};
			D3DXMatrixRotationYawPitchRoll(&RotationMatrix, 0.f, D3DXToRadian(150.f), 0.f);
			m_LocalMatrix = m_LocalScaleMatrix * RotationMatrix * m_LocalTransformMatrix;
		}
	}
	else
		m_LocalMatrix = m_LocalScaleMatrix * m_LocalRotationMatrix * m_LocalTransformMatrix;
}

CThirdee_Arm* CThirdee_Arm::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CThirdee_Arm* pInstance = new CThirdee_Arm(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created : CThirdee_Arm"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThirdee_Arm::Clone(void* pArg)
{
	MSG_BOX(TEXT("CThirdee_Arm Class is Can't Cloned"));

	return nullptr;
}

void CThirdee_Arm::Free()
{
	__super::Free();
}
