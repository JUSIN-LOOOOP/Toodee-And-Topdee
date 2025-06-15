#include "Toodoo_Ending.h"
#include "GameInstance.h"

CToodoo_Ending::CToodoo_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CToodoo_Ending::CToodoo_Ending(const CToodoo_Ending& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CToodoo_Ending::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CToodoo_Ending::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 1000.0f;
	m_fSizeY = 500.0f;
	m_fX = 900.f;
	m_fY = 400.f;

	return S_OK;
}

void CToodoo_Ending::Priority_Update(_float fTimeDelta)
{
}

void CToodoo_Ending::Update(_float fTimeDelta)
{
	m_fX += 0.5f;
}

void CToodoo_Ending::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CToodoo_Ending::Render()
{
	if (FAILED(__super::Begin(m_pTransformCom)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();

	if (FAILED(__super::End()))
		return E_FAIL;

	return S_OK;
}

HRESULT CToodoo_Ending::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	return S_OK;
}

HRESULT CToodoo_Ending::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CToodoo_Ending::Ready_Components()
{
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_Component_Texture_Toodoo_Ending"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CToodoo_Ending* CToodoo_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CToodoo_Ending* pInstance = new CToodoo_Ending(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : CToodoo_Ending"));
	}

	return pInstance;
}

CGameObject* CToodoo_Ending::Clone(void* pArg)
{
	CToodoo_Ending* pInstance = new CToodoo_Ending(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Cloned : CToodoo_Ending"));
	}

	return pInstance;
}

void CToodoo_Ending::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
