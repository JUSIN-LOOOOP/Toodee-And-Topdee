#include "BackTile.h"
#include "GameInstance.h"
#include "Util.h"

CBackTile::CBackTile(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CGameObject{ pGraphic_Device }
{
} 

CBackTile::CBackTile(const CBackTile& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CBackTile::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackTile::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pSize[0] = *static_cast<_uint*>(pArg);
    m_pSize[1] = *(static_cast<_uint*>(pArg) + 1);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    m_pTransformCom->Scaling(2.f, 2.f, 2.f);

    return S_OK;
}

void CBackTile::Priority_Update(_float fTimeDelta)
{
}

void CBackTile::Update(_float fTimeDelta)
{

}

void CBackTile::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CBackTile::Render()
{
    if (FAILED(m_pTextureCom->Bind_Texture(0)))
        return E_FAIL;


    for (_uint height = 0; height < m_pSize[1]; ++height)
    {
        for (_uint width = 0; width < m_pSize[0]; ++width)
        {
            m_pTransformCom->Set_State(STATE::POSITION, { (FLOAT)((width + 0.5 - (m_pSize[0] / 2)) * 2) , 0.f,  (FLOAT)((height + 0.5 - (m_pSize[1] / 2)) * 2) });
            m_pTransformCom->Bind_Matrix();
            m_pVIBufferCom->Bind_Buffers();
            Begin_RenderState();
            m_pVIBufferCom->Render();
        }
    }
    End_RenderState();
    
    return S_OK;
}

HRESULT CBackTile::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}

HRESULT CBackTile::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

HRESULT CBackTile::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BackTile"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;
    return S_OK;
}

CBackTile* CBackTile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackTile* pInstance = new CBackTile(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBackTile"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBackTile* CBackTile::Clone(void* pArg)
{

    CBackTile* pInstance = new CBackTile(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBackTile"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackTile::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
