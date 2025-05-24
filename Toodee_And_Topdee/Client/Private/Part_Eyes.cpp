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
	, m_fTheta{ Prototype.m_fTheta }
	, m_fPhi{ Prototype.m_fPhi }
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

	EYEPART_DESC* pDesc = reinterpret_cast<EYEPART_DESC*>(pArg);

	m_fTheta = pDesc->m_fTheta;
	m_fPhi = pDesc->m_fPhi;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);

	_float3 vWorldPos = pDesc->pParentTransform->Get_State(STATE::POSITION);

	_float3 vRadiusX = (pDesc->pParentTransform)->Get_State(STATE::RIGHT) / 2;
	_float3 vRadiusY = (pDesc->pParentTransform)->Get_State(STATE::UP) / 2;
	_float3 vRadiusZ = (pDesc->pParentTransform)->Get_State(STATE::LOOK) / 2;


	_float fRadiusX = fabsf(D3DXVec3Length(&vRadiusX));
	_float fRadiusY = fabsf(D3DXVec3Length(&vRadiusY));
	_float fRadiusZ = fabsf(D3DXVec3Length(&vRadiusZ));

	_float3 vPos = {};
	vPos.x = vWorldPos.x + fRadiusX * sinf(m_fTheta) * cos(m_fPhi);
	vPos.y = vWorldPos.y + fRadiusY * cosf(m_fTheta);
	vPos.z = vWorldPos.z + fRadiusZ * sinf(m_fTheta) * sin(m_fPhi);

	// vWorldPos += vPos;

	m_pTransformCom->Set_State(STATE::POSITION, vPos);
	return S_OK;
}

void CPart_Eyes::Priority_Update()
{

}

void CPart_Eyes::Update(class CTransform* pTransform, _float fTheta, _float fPhi)
{
	
	m_fTheta += D3DXToRadian(fTheta);
	m_fPhi += D3DXToRadian(fPhi);



	_float3 vWorldPos = pTransform->Get_State(STATE::POSITION);

	_float3 vRadiusX = pTransform->Get_State(STATE::RIGHT);
	_float3 vRadiusY = pTransform->Get_State(STATE::UP);
	_float3 vRadiusZ = pTransform->Get_State(STATE::LOOK);

	_float fRadiusX = D3DXVec3Length(&vRadiusX) / 2;
	_float fRadiusY = D3DXVec3Length(&vRadiusY) / 2 ;
	_float fRadiusZ = D3DXVec3Length(&vRadiusZ) / ds2 ;

	_float3 vPos = {};
	vPos.x = vWorldPos.x + fRadiusX * sinf(m_fTheta + D3DXToRadian(fTheta)) * cosf(m_fPhi + D3DXToRadian(fPhi));
	vPos.y = vWorldPos.y + fRadiusY * cosf(m_fTheta + D3DXToRadian(fTheta));
	vPos.z = vWorldPos.z + fRadiusZ * sinf(m_fTheta + D3DXToRadian(fTheta)) * sinf(m_fPhi + D3DXToRadian(fPhi));

	// _float fRadiusX{}, fRadiusY{};
	//  
	// fRadiusX = fabsf(vWorldPos.x / 2);
	// fRadiusY = fabsf(vWorldPos.y / 2);
	//  
	// _float3 vLocalPos = {};
	// D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, m_pTransformCom->Get_WorldMatrix_Inverse());
	
	// view행렬을 이용한 빌보딩

	
	
	// 부모x + x의 반지름 * sin(각도)
	// 부모y + y의 반지름 * cos(각도)
	// 부모z + z의 반지름 * sin(각도)


	// vWorldPos += vPos;

	m_pTransformCom->Set_State(STATE::POSITION, vPos);

	// _float4x4 ViewMatrix = {};
	// 
	// m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	// 
	// D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	// 
	// _float3 fCameraPos = _float3(ViewMatrix._41, ViewMatrix._42, ViewMatrix._43);
	// 
	// m_pTransformCom->Look_At(*reinterpret_cast<_float3*>(&ViewMatrix.m[3]));
	// 
	

}

void CPart_Eyes::Late_Update()
{
}

HRESULT CPart_Eyes::Render()
{
	// m_pTransformCom->Set_State(STATE::POSITION, m_vPos);
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(2)))
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
		MSG_BOX(TEXT("Failed To Created : Part_Eyes"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

CComponent* CPart_Eyes::Clone(void* pArg)
{
	CPart_Eyes* pInstance = new CPart_Eyes(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : Part_Eyes"));
		Safe_Release(pInstance);

	}

	return pInstance;
}

void CPart_Eyes::Free()
{
	__super::Free();


}
