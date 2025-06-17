#include "FinalTile.h"
#include "GameInstance.h"

CFinalTile::CFinalTile(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject {pGraphic_Device }
{
}

CFinalTile::CFinalTile(const CFinalTile& Prototype)
	: CBlendObject { Prototype }
{
}

HRESULT CFinalTile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFinalTile::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);
	pDesc->vPos.y -= 1.f;

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
	
	return S_OK;
}

void CFinalTile::Priority_Update(_float fTimeDelta)
{
}

void CFinalTile::Update(_float fTimeDelta)
{
}

void CFinalTile::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_TILE, this);
	__super::Compute_CamDistance(m_pTransformCom);
}

HRESULT CFinalTile::Render()
{

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pTransformCom->Bind_Matrix();

	m_pVIBufferCom->Bind_Buffers();
	
	Begin_RenderState();
	
	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}

HRESULT CFinalTile::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Tile"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFinalTile::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CFinalTile::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	
	return S_OK;
}

CFinalTile* CFinalTile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFinalTile* pInstance = new CFinalTile(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : CFinalTile"));
	}

	return pInstance;
}

CGameObject* CFinalTile::Clone(void* pArg)
{

	CFinalTile* pInstance = new CFinalTile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Cloned : CFinalTile"));
	}

	return pInstance;
}

void CFinalTile::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
