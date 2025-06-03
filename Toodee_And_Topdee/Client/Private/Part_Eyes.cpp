#include "Part_Eyes.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Pig.h"

CPart_Eyes::CPart_Eyes(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Eyes::CPart_Eyes(const CPart_Eyes& Prototype)
	: CParts {Prototype}
	, m_iTextureIndex{ Prototype.m_iTextureIndex }
{
}


HRESULT CPart_Eyes::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Eyes::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;
	m_strTexTag = pDesc->strTexTag;
	m_eState = pDesc->eState;
	m_vBodyScale = pDesc->vBodyScale;
	m_iTexLevelIndex = pDesc->iTexLevelIndex;

	if (pDesc->eState == PARTSTATE::PARTS_RIGHT)
	{
		m_fAngleX = -(pDesc->fAngleX);
		m_fAngleY = -(pDesc->fAngleY);
	}
	else
	{
		m_fAngleX = pDesc->fAngleX;
		m_fAngleY = pDesc->fAngleY;
	}

	m_pVIBufferCom = pDesc->pVIBufferCom;

	Safe_AddRef(m_pVIBufferCom);

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_Transform")));


	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, m_iTexLevelIndex, m_strTexTag));

	Ready_Component();
	return S_OK;
}


void CPart_Eyes::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	_float3 vMyPos = pTransform->Get_State(STATE::POSITION);
	__super::Check_To_FocusDelta(&m_iDeltaAngleX, &m_iDeltaAngleY, vFocusPos, vMyPos);
	__super::RevolveAround(pTransform, m_iDeltaAngleX, m_iDeltaAngleY, -0.3f);

	if (m_eState == PARTSTATE::PARTS_RIGHT)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
	
}

HRESULT CPart_Eyes::Render(void* pArg)
{

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CPart_Eyes::Ready_Component()
{

}

CPart_Eyes* CPart_Eyes::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Eyes* pInstance = new CPart_Eyes(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Eyes"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Eyes::Clone(void* pArg)
{
	CPart_Eyes* pInstance = new CPart_Eyes(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Eyes"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Eyes::Free()
{
	__super::Free();

	// Safe_Release(m_pVIBufferCom);
	// Safe_Release(m_pTransformCom);
	// Safe_Release(m_pTextureCom);
}
