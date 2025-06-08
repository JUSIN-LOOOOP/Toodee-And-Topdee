#include "Toodoo.h"
#include "GameInstance.h"
#include "Util.h"

CToodoo::CToodoo(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CGameObject { pGraphic_Device }
{
} 

CToodoo::CToodoo(const CToodoo& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CToodoo::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CToodoo::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CToodoo::Priority_Update(_float fTimeDelta)
{

}

void CToodoo::Update(_float fTimeDelta)
{
    m_fDeltaTime += fTimeDelta;

    if (m_fDeltaTime >= 1.5f)
    {
        offsetFlag = (offsetFlag == false) ? true : false;
        m_fDeltaTime = 0;
    }

    m_fOffset += (offsetFlag == false) ? -fTimeDelta : fTimeDelta;
}

void CToodoo::Late_Update(_float fTimeDelta)
{
    _float4x4 ViewMatrix{};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    _float3 ViewPos  = { ViewMatrix._41, ViewMatrix._42, ViewMatrix._43 };
    _float3 vRight   = { ViewMatrix._11, ViewMatrix._12, ViewMatrix._13 };
    _float3 vUp      = { ViewMatrix._21, ViewMatrix._22, ViewMatrix._23 };
    _float3 vLook    = { ViewMatrix._31, ViewMatrix._32, ViewMatrix._33 };

    D3DXVec3Normalize(&vLook, &vLook);

    _float3 vOffset = vRight * (2.f) + vUp * m_fOffset;

    _float3 Pos = ViewPos + vLook * 8.f + vOffset;
    m_pTransformCom->Set_State(STATE::POSITION, Pos);

    m_pTransformCom->Set_State(STATE::RIGHT, vRight * 12.f);
    m_pTransformCom->Set_State(STATE::UP,    vUp    * 9.f);
    m_pTransformCom->Set_State(STATE::LOOK,  vLook);
   
    vRight = { ViewMatrix._11, ViewMatrix._12, ViewMatrix._13 };
    vUp = { ViewMatrix._21, ViewMatrix._22, ViewMatrix._23 };
    vLook = { ViewMatrix._31, ViewMatrix._32, ViewMatrix._33 };

    m_pTransformCom->Set_State(STATE::RIGHT, vRight * 12.f);
    m_pTransformCom->Set_State(STATE::UP, vUp * 9.f);
    m_pTransformCom->Set_State(STATE::LOOK, vLook );

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CToodoo::Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(0))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    return S_OK;
}

HRESULT CToodoo::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodoo"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CToodoo* CToodoo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CToodoo* pInstance = new CToodoo(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CToodoo"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CToodoo* CToodoo::Clone(void* pArg)
{

    CToodoo* pInstance = new CToodoo(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CToodoo"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CToodoo::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}

