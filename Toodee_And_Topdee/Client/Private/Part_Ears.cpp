#include "Part_Ears.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Pig.h"

CPart_Ears::CPart_Ears(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Ears::CPart_Ears(const CPart_Ears& Prototype)
	: CParts {Prototype}
	, m_iTextureIndex{ Prototype.m_iTextureIndex }
{
}


HRESULT CPart_Ears::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Ears::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CParts::PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}


void CPart_Ears::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	//이 조건문은 Pig기준이라 혹시 Bat배치시 변경이 있으면 상황에 맞는 방향으로 예외처리 수정할게
	_float3 vPos = pTransform->Get_State(STATE::POSITION);
	Turn_To_Ears(vFocusPos.x, vPos.x, fTimeDelta);

	__super::RevolveAround(pTransform, static_cast<_int>(m_fDeltaAngleX), 0);

	if (m_fDeltaAngleX < 0)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
}

HRESULT CPart_Ears::Render(void* pArg)
{

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CPart_Ears::Turn_To_Ears(_float fFocusPosX, _float fMyPosX, _float fTimeDelta)
{
	_float fRadiusX = m_vBodyScale.x * 0.5f;
	if (fMyPosX + fRadiusX > fFocusPosX && m_fDeltaAngleX > -5)
		m_fDeltaAngleX += -0.1f;
	else if (fMyPosX - fRadiusX < fFocusPosX && m_fDeltaAngleX < 5)
		m_fDeltaAngleX += 0.1f;
}

CPart_Ears* CPart_Ears::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Ears* pInstance = new CPart_Ears(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Ears"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Ears::Clone(void* pArg)
{
	CPart_Ears* pInstance = new CPart_Ears(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Ears"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Ears::Free()
{
	__super::Free();


}
