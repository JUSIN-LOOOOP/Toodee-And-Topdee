#include "Parts.h"
#include "GameInstance.h"

CParts::CParts(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{pGraphic_Device}
{
}

CParts::CParts(const CParts& Prototype)
	: CComponent{Prototype}
	, m_eState			{ Prototype.m_eState }
	, m_iTexLevelIndex	{Prototype.m_iTexLevelIndex }
	, m_strTexTag		{ Prototype.m_strTexTag }
	, m_pParentObject	{ Prototype.m_pParentObject }
	, m_pVIBufferCom	{ Prototype.m_pVIBufferCom }
	, m_pTransformCom	{ Prototype.m_pTransformCom }
	, m_pTextureCom		{ Prototype.m_pTextureCom  }

{

}

HRESULT CParts::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParts::Initialize(void* pArg)
{
	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);

	m_strTexTag = pDesc->strTexTag;
	m_eState = pDesc->eState;
	m_iTexLevelIndex = pDesc->iTexLevelIndex;

	m_pParentObject = pDesc->pParentObject;
	
	m_pVIBufferCom = static_cast<CVIBuffer_Rect*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_VIBuffer_Rect")));

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, 0, TEXT("Prototype_Component_Transform")));

	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, m_iTexLevelIndex, m_strTexTag));

	return S_OK;
}

void CParts::Priority_Update()
{
}

void CParts::Update(class CTransform* pTransform, _float fTheta, _float fPhi)
{
}

void CParts::Late_Update()
{
}

HRESULT CParts::Render()
{
	
	return S_OK;
}

void CParts::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

}
