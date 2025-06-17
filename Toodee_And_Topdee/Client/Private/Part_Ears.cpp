#include "Part_Ears.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Pig.h"

CPart_Ears::CPart_Ears(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParts {pGraphic_Device}
{
}

CPart_Ears::CPart_Ears(const CPart_Ears& Prototype)
	: CParts {Prototype}
{
}


HRESULT CPart_Ears::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Ears::Initialize(void* pArg)
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


void CPart_Ears::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	//이 조건문은 Pig기준이라 혹시 Bat배치시 변경이 있으면 상황에 맞는 방향으로 예외처리 수정할게
	if (m_strOtherName.find(TEXT("Pig")) != string::npos)
	{
		_float3 vPos = pTransform->Get_State(STATE::POSITION);
		_float3 vScale = pTransform->Get_Scaled();
		if (vPos.x + vScale.x * 0.5f > vFocusPos.x && m_fDeltaAngleX > -5)
			m_fDeltaAngleX += -0.1f;
		else if (vPos.x + vScale.x * 0.5f < vFocusPos.x && m_fDeltaAngleX < 5)
			m_fDeltaAngleX += 0.1f;
	}
	

	__super::RevolveAround(pTransform, m_fDeltaAngleX, 0);

	if (m_strOtherName.find(TEXT("Bat")) != string::npos && m_eState == PARTSTATE::PARTS_RIGHT)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));

	if (m_strOtherName.find(TEXT("Pig")) != string::npos && m_fDeltaAngleX < 0)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
}

HRESULT CPart_Ears::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

CPart_Ears* CPart_Ears::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPart_Ears* pInstance = new CPart_Ears(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPart_Ears"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CPart_Ears::Clone(void* pArg)
{
	CPart_Ears* pInstance = new CPart_Ears(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPart_Ears"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPart_Ears::Free()
{
	__super::Free();

}
