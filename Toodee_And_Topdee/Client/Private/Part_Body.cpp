#include "Part_Body.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Pig.h"

CPart_Body::CPart_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Body::CPart_Body(const CPart_Body& Prototype)
	: CParts {Prototype}
	, m_iTextureIndex{ Prototype.m_iTextureIndex }
{
}


HRESULT CPart_Body::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Body::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_iTextureIndex = pDesc->iTextureIndex;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}


void CPart_Body::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	Pos_Set(pTransform);

	m_fFrame += 9 * fTimeDelta;
	if (m_fFrame >= 9)
		m_fFrame = 1;
}

HRESULT CPart_Body::Render(void* pArg)
{
	if (nullptr != pArg)
	{
		m_pTransformCom->Bind_Matrix();

		if (FAILED(m_pTextureCom->Bind_Texture(static_cast<_uint>(m_fFrame))))
			return E_FAIL;

		m_pVIBufferCom->Bind_Buffers();

		m_pVIBufferCom->Render();
	}
	else
	{
		m_pTransformCom->Bind_Matrix();

		if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIndex)))
			return E_FAIL;

		m_pVIBufferCom->Bind_Buffers();

		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CPart_Body::Pos_Set(CTransform* pTransform)
{
	_float3 vRight = pTransform->Get_State(STATE::RIGHT);
	_float3 vUp = pTransform->Get_State(STATE::UP);
	_float3 vLook = pTransform->Get_State(STATE::LOOK);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);
	vRight *= m_vBodyScale.x;
	vUp *= m_vBodyScale.y;
	vLook *= m_vBodyScale.z;
	m_pTransformCom->Set_State(STATE::RIGHT, vRight);
	m_pTransformCom->Set_State(STATE::UP, vUp);
	m_pTransformCom->Set_State(STATE::LOOK, vLook);
	m_pTransformCom->Set_State(STATE::POSITION, pTransform->Get_State(STATE::POSITION));
}

CPart_Body* CPart_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Body* pInstance = new CPart_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Body"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Body::Clone(void* pArg)
{
	CPart_Body* pInstance = new CPart_Body(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Body"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Body::Free()
{
	__super::Free();


}
