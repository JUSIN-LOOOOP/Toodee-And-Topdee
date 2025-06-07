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
{
}


HRESULT CPart_Body::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Body::Initialize(void* pArg)
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


void CPart_Body::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	Pos_Set(pTransform);

	if(m_fMaxFrame != 0)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta;
		if (m_fFrame >= m_fMaxFrame)
			m_fFrame = m_fOldFrame;
	}
}

HRESULT CPart_Body::Render()
{

	m_pTransformCom->Bind_Matrix();
	
	
	if (m_strOtherName.find(TEXT("Pig")) != string::npos)
	{
		switch (m_pGameInstance->Get_CurrentDimension())
		{
		case::DIMENSION::TOODEE:
			if (FAILED(m_pTextureCom->Bind_Texture(static_cast<_uint>(m_fFrame))))		
				return E_FAIL;
			break;
		case::DIMENSION::TOPDEE:
			if (FAILED(m_pTextureCom->Bind_Texture(0)))
				return E_FAIL;
			break;
		}
	}
	else
	{
		if (FAILED(m_pTextureCom->Bind_Texture(0)))
			return E_FAIL;
	}
	
	
	m_pVIBufferCom->Bind_Buffers();
	
	m_pVIBufferCom->Render();

	return S_OK;
}

void CPart_Body::Pos_Set(CTransform* pTransform)
{
	m_pTransformCom->Set_State(STATE::RIGHT, pTransform->Get_State(STATE::RIGHT));
	m_pTransformCom->Set_State(STATE::UP, pTransform->Get_State(STATE::UP));
	m_pTransformCom->Set_State(STATE::LOOK, pTransform->Get_State(STATE::LOOK));
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
