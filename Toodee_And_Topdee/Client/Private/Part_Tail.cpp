#include "Part_Tail.h"
#include "GameInstance.h"
#include "GameObject.h"

CPart_Tail::CPart_Tail(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Tail::CPart_Tail(const CPart_Tail& Prototype)
	: CParts{ Prototype }
{
}


HRESULT CPart_Tail::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CPart_Tail::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


void CPart_Tail::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	_float3 vMyPos = pTransform->Get_State(STATE::POSITION);
	__super::Check_To_FocusDelta(&m_iDeltaAngleX, &m_iDeltaAngleY, vFocusPos, vMyPos);
	__super::RevolveAround(pTransform, m_iDeltaAngleX, m_iDeltaAngleY, 0.2f);

	if (m_iDeltaAngleX < 0)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
}

HRESULT CPart_Tail::Render(void* pArg)
{
	
	m_pTransformCom->Bind_Matrix();
	
	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

CPart_Tail* CPart_Tail::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Tail* pInstance = new CPart_Tail(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Tail"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Tail::Clone(void* pArg)
{
	CPart_Tail* pInstance = new CPart_Tail(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Tail"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Tail::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
