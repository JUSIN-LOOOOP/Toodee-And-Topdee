#include "Backdrop.h"
#include "GameInstance.h"
#include "Util.h"

CBackdrop::CBackdrop(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CGameObject { pGraphic_Device }
{
} 

CBackdrop::CBackdrop(const CBackdrop& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CBackdrop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackdrop::Initialize(void* pArg)
{
    m_iThemeIdx = *(static_cast<_uint*>(pArg));

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CBackdrop::Priority_Update(_float fTimeDelta)
{

}

void CBackdrop::Update(_float fTimeDelta)
{

}

void CBackdrop::Late_Update(_float fTimeDelta)
{
    _float4x4       WorldMatrix, ViewMatrix, localTrans{};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    _float3 ViewPos = { ViewMatrix._41,  ViewMatrix._42,  ViewMatrix._43 };
    _float3 ViewLook = { ViewMatrix._31,  ViewMatrix._32,  ViewMatrix._33 };
    D3DXVec3Normalize(&ViewLook, &ViewLook);

    _float3 Pos = ViewPos + ViewLook * 10.f;
    m_pTransformCom->Set_State(STATE::POSITION, Pos);

    _float3 vRight = { ViewMatrix._11, ViewMatrix._12, ViewMatrix._13 };
    _float3 vUp = { ViewMatrix._21, ViewMatrix._22, ViewMatrix._23 };
    _float3 vLook = { ViewMatrix._31, ViewMatrix._32, ViewMatrix._33 };

    m_pTransformCom->Set_State(STATE::RIGHT, vRight * 21.f);
    m_pTransformCom->Set_State(STATE::UP, vUp * 11.6f);
    m_pTransformCom->Set_State(STATE::LOOK, vLook );

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);

    /*if (m_iThemeIdx == 3)
    {
        m_fMoveOffset += fTimeDelta;
        
    }*/
}

HRESULT CBackdrop::Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(m_iThemeIdx))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    /*const _float4x4* world = m_pTransformCom->Get_WorldMatrix();

    if (m_iThemeIdx == 3)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

        _float3 backupPos = m_pTransformCom->Get_State(STATE::POSITION);

        m_pTransformCom->Scaling(18.f, 14.f, 1.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3{ backupPos.x, backupPos.y - 5.f, backupPos.z - 5.f });
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pBossTextureCom->Bind_Texture(ENUM_CLASS(0))))
            return E_FAIL;
        m_pVIBufferCom->Bind_Buffers();
        m_pVIBufferCom->Render();
        m_pTransformCom->Set_State(STATE::POSITION, _float3{ backupPos.x, backupPos.y, backupPos.z });

        m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }*/

    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    return S_OK;
}

HRESULT CBackdrop::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Backdrop"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    if (m_iThemeIdx == 3)
        if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodoo"),
            TEXT("Com_ToodooTexture"), reinterpret_cast<CComponent**>(&m_pBossTextureCom))))
            return E_FAIL;

    return S_OK;
}

CBackdrop* CBackdrop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackdrop* pInstance = new CBackdrop(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBackdrop"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBackdrop* CBackdrop::Clone(void* pArg)
{

    CBackdrop* pInstance = new CBackdrop(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBackdrop"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackdrop::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pBossTextureCom);
}
