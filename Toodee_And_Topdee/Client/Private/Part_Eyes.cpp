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
{
}


HRESULT CPart_Eyes::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CPart_Eyes::Initialize(void* pArg)
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


void CPart_Eyes::Update(CTransform* pTransform, _float fTimeDelta, _float3 vFocusPos)
{
	_float3 vMyPos = pTransform->Get_State(STATE::POSITION);
	
	__super::Look_At_degree(&m_fDeltaAngleX, &m_fDeltaAngleY, pTransform, vFocusPos);

	__super::RevolveAround(pTransform, m_fDeltaAngleX, m_fDeltaAngleY, -0.3f, -0.3f);
	 
	if (m_eState == PARTSTATE::PARTS_RIGHT)
		m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
	
}

HRESULT CPart_Eyes::Render()
	{

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	return S_OK;
}

void CPart_Eyes::Look_At_degree(_float& pAngleXOut, _float& pAngleYOut, CTransform* pTransform, _float3 vTargetPos)
{
	
	_float3 vDeltaPos = vTargetPos - pTransform->Get_State(STATE::POSITION);
	_float3 vRight = pTransform->Get_State(STATE::RIGHT);
	_float3 vUp = pTransform->Get_State(STATE::UP);
	_float3 vLook = pTransform->Get_State(STATE::LOOK);

	_float3 vDeltaX = vDeltaPos;
	_float3 vUpX = vUp;
	_float3 vDeltaY = vDeltaPos;
	_float3 vRightY = vRight;
	_float3 vLookY = vLook;
	// z축 제거 (x축을 수평기준 각도체크)
	vDeltaX.y = 0.f; vUpX.y = 0.f;
	// x축 제거 (z축을 수평기준 각도체크)
	vDeltaY.y = 0.f; vRightY.y = 0.f;

	vUpX *= -1.f;

	D3DXVec3Normalize(&vDeltaX, &vDeltaX);
	D3DXVec3Normalize(&vDeltaY, &vDeltaY);
	D3DXVec3Normalize(&vUpX, &vUpX);
	D3DXVec3Normalize(&vRightY, &vRightY);


	_float fDotX = D3DXVec3Dot(&vUpX, &vDeltaX);
	_float fDegreeX = D3DXToDegree(acosf(fDotX));

	

	_float fDotY = D3DXVec3Dot(&vRightY, &vDeltaY);
	_float fDegreeY = D3DXToDegree(acosf(fDotY));

	_int a = 10;
	
	if (vDeltaPos.x < 0)	fDegreeX = -fDegreeX;
	// 
	if (vDeltaPos.z < 0)	fDegreeY = -fDegreeY;
	if (fDegreeY <= -20.f)	fDegreeY = -20.f; // 하단부방향에 대한 범위제한(상하)
	if (fDegreeY >= 135.f && fDegreeX < -90.f) fDegreeX *= -1.f; // 타겟이 왼쪽 상단일경우 
	if (fDegreeY >= 45 && fDegreeY <= 135) // 60~120(타겟이 위쪽일경우 범위제한
	{
		if (fDegreeX <= -90.f)
			fDegreeX *= -1.f;
		fDegreeY = 30.f;
	}

		
	
	
	pAngleXOut = fDegreeX;
	pAngleYOut = fDegreeY;
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
