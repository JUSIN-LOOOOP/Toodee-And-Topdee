#include "Part_Legs.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Pig.h"

CPart_Legs::CPart_Legs(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Legs::CPart_Legs(const CPart_Legs& Prototype)
	: CParts {Prototype}
	, m_iTextureIndex{ Prototype.m_iTextureIndex }
{
}


HRESULT CPart_Legs::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Legs::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}


void CPart_Legs::Update(CTransform* pTransform, _float fTimeDelta, _float InputWidth, _float InputDepth)
{
	__super::RevolveAround(pTransform, m_fWidth, m_fDepth);
	m_fFrame += 9 * fTimeDelta;
	if (m_fFrame >= 8)
		m_fFrame = 0;
}

HRESULT CPart_Legs::Render(void* pArg)
{
	m_pTransformCom->Bind_Matrix();
	// switch (m_eTexState)
	// {
	// 	TEXTUREDIRECTION
	// }
	if (FAILED(m_pTextureCom->Bind_Texture(static_cast<_uint>(m_fFrame))))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

CPart_Legs* CPart_Legs::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Legs* pInstance = new CPart_Legs(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Legs"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Legs::Clone(void* pArg)
{
	CPart_Legs* pInstance = new CPart_Legs(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Legs"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Legs::Free()
{
	__super::Free();


}
