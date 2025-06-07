#include "Part_Nose.h"
#include "GameInstance.h"
#include "GameObject.h"

CPart_Nose::CPart_Nose(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Nose::CPart_Nose(const CPart_Nose& Prototype)
	: CParts{ Prototype }
{
}


HRESULT CPart_Nose::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CPart_Nose::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_strOtherName = pDesc->strOtherName;
	m_eState = pDesc->eState;
	m_fFrame = pDesc->fFrame;
	m_fMaxFrame = pDesc->fMaxFrame;

	if (pDesc->eState == PARTSTATE::PARTS_RIGHT)
	{	m_fAngleX = -(pDesc->fAngleX);	m_fAngleY = -(pDesc->fAngleY);	}
	else
	{	m_fAngleX = pDesc->fAngleX;		m_fAngleY = pDesc->fAngleY;	}

	m_pVIBufferCom = pDesc->pVIBufferCom;
	m_pTextureCom = pDesc->pTextureCom;

	Safe_AddRef(m_pVIBufferCom);

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform")));




	return S_OK;
}


void CPart_Nose::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	_float3 vMyPos = pTransform->Get_State(STATE::POSITION);
	__super::Look_At_degree(&m_fDeltaAngleX, &m_fDeltaAngleY, pTransform, vFocusPos);


	__super::RevolveAround(pTransform, m_fDeltaAngleX, m_fDeltaAngleY, 0.1f, -0.2f);
}

HRESULT CPart_Nose::Render()
{
	m_pTransformCom->Bind_Matrix();


	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

CPart_Nose* CPart_Nose::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Nose* pInstance = new CPart_Nose(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Nose"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

CComponent* CPart_Nose::Clone(void* pArg)
{
	CPart_Nose* pInstance = new CPart_Nose(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Nose"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

void CPart_Nose::Free()
{
	__super::Free();

}
