#include "Part_Wing.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Pig.h"

CPart_Wing::CPart_Wing(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Wing::CPart_Wing(const CPart_Wing& Prototype)
	: CParts {Prototype}
{
}


HRESULT CPart_Wing::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Wing::Initialize(void* pArg)
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

	m_fOldFrame = m_fFrame;

	return S_OK;
}


void CPart_Wing::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{

	m_fFrame += m_fMaxFrame * fTimeDelta;
	if (m_fFrame >= m_fMaxFrame)
		m_fFrame = m_fOldFrame;

	_float3 vMyPos = pTransform->Get_State(STATE::POSITION);
	__super::RevolveAround(pTransform, m_iDeltaAngleX, m_iDeltaAngleY, 0.3f);
	m_pTransformCom->Adjust_Scale(_float3(1.f, 2.f, 1.f));
	if (m_eState == PARTSTATE::PARTS_RIGHT)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
	
}

HRESULT CPart_Wing::Render(void* pArg)
{

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(static_cast<_uint>(m_fFrame))))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

CPart_Wing* CPart_Wing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Wing* pInstance = new CPart_Wing(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Wing"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Wing::Clone(void* pArg)
{
	CPart_Wing* pInstance = new CPart_Wing(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Wing"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Wing::Free()
{
	__super::Free();

}
