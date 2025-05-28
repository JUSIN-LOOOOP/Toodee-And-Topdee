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
		return E_FAIL;

	CParts::PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}


void CPart_Eyes::Update(CTransform* pTransform, _float fTimeDelta, _float InputWidth, _float InputDepth)
{
	_float Scale = 0.7f;
	__super::If_Revolved(m_fWidth, m_fDepth, InputWidth, InputDepth);
	__super::RevolveAround(pTransform, m_fWidth, m_fDepth, Scale);
	
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


}
