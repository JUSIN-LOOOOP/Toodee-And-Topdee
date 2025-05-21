#include "Camera.h"
#include "GameInstance.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vEye);
	m_pTransformCom->Look_At(pDesc->vAt);

	m_fFovy = pDesc->fFovy;
	m_fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	m_fMouseSensor = pDesc->fMouseSensor;



	/*GetCursorPos(&m_OldPoint);
	ScreenToClient(g_hWnd, &m_OldPoint);*/

	m_OldPoint.x = g_iWinSizeX >> 1;
	m_OldPoint.y = g_iWinSizeY >> 1;

	POINT		ptMouse = m_OldPoint;
	ClientToScreen(g_hWnd, &ptMouse);

	SetCursorPos(ptMouse.x, ptMouse.y);

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	if (GetKeyState('W') < 0)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('S') < 0)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (GetKeyState('A') < 0)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (GetKeyState('D') < 0)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_int		iMouseMove = {};

	if (iMouseMove = ptMouse.x - m_OldPoint.x)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), iMouseMove * fTimeDelta * m_fMouseSensor);
	}

	if (iMouseMove = ptMouse.y - m_OldPoint.y)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), iMouseMove * fTimeDelta * m_fMouseSensor);
	}

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));

	m_OldPoint = ptMouse;
}

void CCamera::Update(_float fTimeDelta)
{


}

void CCamera::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera::Render()
{

	return S_OK;
}

HRESULT CCamera::Ready_Components(void* pArg)
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera* pInstance = new CCamera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
