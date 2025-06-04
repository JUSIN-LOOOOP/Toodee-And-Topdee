#include "TextureUI.h"
#include "GameInstance.h"

CTextureUI::CTextureUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CTextureUI::CTextureUI(const CTextureUI& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTextureUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTextureUI::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(40.f, 40.f, 1.f);
	//m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	D3DXMatrixIdentity(&m_ViewMatrix);
	D3DXMatrixOrthoLH(&m_ProjMatrix, g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	return S_OK;
}

void CTextureUI::Priority_Update(_float fTimeDelta)
{
	BLOCK_INFO arg = m_pGameInstance->Get_CurrentType();
	m_iTextureIdx = arg.iTileTextureIdx;
}

void CTextureUI::Update(_float fTimeDelta)
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= g_iWinSizeX / 2;
	ptMouse.y = static_cast<_long>(-((float)ptMouse.y - float(g_iWinSizeY / 2)));
	m_pTransformCom->Set_State(STATE::POSITION, _float3((float)ptMouse.x, (float)ptMouse.y, 0.f));

}

void CTextureUI::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CTextureUI::Render()
{
	m_pTransformCom->Bind_Matrix();

	_float4x4			OldViewMatrix, OldProjMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


	if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureIdx)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	return S_OK;
	}

HRESULT CTextureUI::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Tile"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CTextureUI::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* 알파 테스트를 수행한다. */
	/* 현재 픽셀을 그릴때 픽셀에 알파값을 비교한다. 통과한 픽셀만 그린다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 현재 그리려고하는 픽셀과 0의 값을 비교한다,ㅡ /.*/
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);

	/* 위에서 비교한 결과, 내가 그릴려고하는 픽셀의 알파가 0보다 크면 통과. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CTextureUI::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

CTextureUI* CTextureUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTextureUI* pInstance = new CTextureUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTextureUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CTextureUI::Clone(void* pArg)
{
	CTextureUI* pInstance = new CTextureUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTextureUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTextureUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

}
