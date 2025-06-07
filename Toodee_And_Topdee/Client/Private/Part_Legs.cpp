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
{
}


HRESULT CPart_Legs::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Legs::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	PART_DESC* pDesc = reinterpret_cast<PART_DESC*>(pArg);
	m_strOtherName = pDesc->strOtherName;
	m_eState = pDesc->eState;
	m_fFrame = pDesc->fFrame;
	m_fMaxFrame = pDesc->fMaxFrame;

	if (pDesc->eState == PARTSTATE::PARTS_RIGHT)
	{	m_fAngleX = -(pDesc->fAngleX);	m_fAngleY = (pDesc->fAngleY);	}
	else
	{	m_fAngleX = pDesc->fAngleX;		m_fAngleY = pDesc->fAngleY;	}

	m_pVIBufferCom = pDesc->pVIBufferCom;
	m_pTextureCom = pDesc->pTextureCom;

	Safe_AddRef(m_pVIBufferCom);

	m_pTransformCom = static_cast<CTransform*>(m_pGameInstance->
		Clone_Prototype(PROTOTYPE::COMPONENT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform")));



	return S_OK;
}


void CPart_Legs::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	


	if(m_strOtherName.find(TEXT("Bat")) != string::npos)
	{
		if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)
		{
			_float3 vOtherPos = pTransform->Get_State(STATE::POSITION);
			_float3 vMyPos = m_pTransformCom->Get_State(STATE::POSITION);
			__super::Look_At_degree(&m_fDeltaAngleX, &m_fDeltaAngleY, pTransform, vFocusPos);
			__super::RevolveAround(pTransform, -m_fDeltaAngleX, 0);

			if (vOtherPos.x < vMyPos.x)
				m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
		}
		else if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
		{
			__super::Look_At_degree(&m_fDeltaAngleX, &m_fDeltaAngleY, pTransform, vFocusPos);
			__super::RevolveAround(pTransform, -m_fDeltaAngleX, 0);
			if (m_fDeltaAngleX > 0) { m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f)); }
		}
	}

	if (m_strOtherName.find(TEXT("Pig")) != string::npos)
	{
		if (m_fMaxFrame != 0)
		{
			m_fFrame += m_fMaxFrame * fTimeDelta;
			if (m_fFrame >= m_fMaxFrame)
				m_fFrame = m_fOldFrame;
		}
	}
	

	
}

HRESULT CPart_Legs::Render()
{

	m_pTransformCom->Bind_Matrix();

	if (m_strOtherName.find(TEXT("Pig")))
	{	if (FAILED(m_pTextureCom->Bind_Texture(static_cast<_uint>(m_fFrame))))		return E_FAIL;	}
	else
	{	if (FAILED(m_pTextureCom->Bind_Texture(0)))		return E_FAIL;	}

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
