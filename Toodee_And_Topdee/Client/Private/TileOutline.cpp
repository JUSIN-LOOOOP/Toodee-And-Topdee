#include "TileOutline.h"
#include "GameInstance.h"
#include "Player_Topdee.h"

CTileOutline::CTileOutline(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CTileOutline::CTileOutline(const CTileOutline& Prototype)
	: CGameObject { Prototype }
{
}

HRESULT CTileOutline::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTileOutline::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CPlayer_Topdee* pPlayer = static_cast<CPlayer_Topdee*>(pArg);

	m_pOwnerPlayer = pPlayer;
	Safe_AddRef(m_pOwnerPlayer);

	Ready_Components();

	m_iMaxAnimCount = 4;
	m_iCurrentAnimCount = 0;
	m_fAnimDelay = 0.05f;
	m_fAnimTime = 0.f;

	m_pTransformCom->Scaling(2.f, 2.f, 0.f);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	return S_OK;
}

void CTileOutline::Priority_Update(_float fTimeDelta)
{
}

void CTileOutline::Update(_float fTimeDelta)
{
	if (m_pOwnerPlayer->InAction() || m_pOwnerPlayer->IsAttach())
	{
		_float3 vPosition = m_pOwnerPlayer->ComputeTileOutlinePosition();

		m_pTransformCom->Set_State(STATE::POSITION, vPosition);
	}
}

void CTileOutline::Late_Update(_float fTimeDelta)
{
	if(m_pOwnerPlayer->InAction() || m_pOwnerPlayer->IsAttach())
	{
		UpdateAnim(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
	}
	else
	{
		m_iCurrentAnimCount = 0;
		m_fAnimTime = 0.f;
	}

}

HRESULT CTileOutline::Render()
{
	m_pTransformCom->Bind_Matrix();

	m_pTextureCom->Bind_Texture(m_iCurrentAnimCount);

	m_pVIBufferCom->Bind_Buffers();

	BeginRenderState();

	m_pVIBufferCom->Render();

	EndRenderState();

	return S_OK;
}

HRESULT CTileOutline::Ready_Components()
{
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_TileOutline"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTileOutline::BeginRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CTileOutline::EndRenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CTileOutline::UpdateAnim(_float fTimeDelta)
{
	if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
	{
		m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
		m_fAnimTime = 0.f;
	}
	else
		m_fAnimTime += fTimeDelta;
}

CTileOutline* CTileOutline::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTileOutline* pInstance = new CTileOutline(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CTileOutline"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTileOutline::Clone(void* pArg)
{
	CTileOutline* pInstance = new CTileOutline(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CTileOutline"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTileOutline::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pOwnerPlayer);
}
