#include "UIObject.h"
#include "Transform.h"

CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{

}

CUIObject::CUIObject(const CUIObject& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CUIObject::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	D3DVIEWPORT9	ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);

	m_fWinSizeX = ViewportDesc.Width;
	m_fWinSizeY = ViewportDesc.Height;

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, m_fWinSizeX, m_fWinSizeY, 0.f, 1.f);

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
	return S_OK;
}

HRESULT CUIObject::Begin(CTransform* pTransform)
{
	_float4x4 WorldMatrix = *pTransform->Get_WorldMatrix();
	D3DXMatrixIdentity(&WorldMatrix);
	pTransform->Set_WorldMatrix(WorldMatrix);

	pTransform->Scaling(m_fSizeX, m_fSizeY, 1.f);
	pTransform->Set_State(STATE::POSITION, _float3(
		m_fX - m_fWinSizeX * 0.5f,
		-m_fY + m_fWinSizeY * 0.5f,
		0.f));

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	pTransform->Bind_Matrix();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	return S_OK;
}

HRESULT CUIObject::End()
{
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	return S_OK;
}

_bool CUIObject::isPicked(HWND hWnd)
{
	RECT	rcRect = { m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f, m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f };

	POINT	ptMouse = {};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	return PtInRect(&rcRect, ptMouse);
}

void CUIObject::Free()
{
	__super::Free();


}
