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
		return E_FAIL;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	return S_OK;
}


void CPart_Nose::Update(CTransform* pTransform, _float fTimeDelta, _float InputWidth, _float InputDepth)
{
	__super::If_Revolved(m_fWidth, m_fDepth, InputWidth, InputDepth);
	__super::RevolveAround(pTransform, m_fWidth, m_fDepth, 0.8f);
}

HRESULT CPart_Nose::Render(void* pArg)
{
	m_pTransformCom->Bind_Matrix();


	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
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
