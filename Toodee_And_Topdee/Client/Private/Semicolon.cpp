#include "Semicolon.h"
#include "GameInstance.h"

CSemicolon::CSemicolon(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CSemicolon::CSemicolon(const CSemicolon& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CSemicolon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSemicolon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_pTransformCom->Scaling(6.f, 15.f, 6.f);
    m_pTransformCom->Set_State(STATE::POSITION, *(static_cast<_float3*>(pArg)));
    m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(90));
    name = TEXT("EnemySemiclon");

    return S_OK;
}

void CSemicolon::Priority_Update(_float fTimeDelta)
{
}

void CSemicolon::Update(_float fTimeDelta)
{
    m_pTransformCom->Go_Backward(fTimeDelta * 15.f);

    _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
    
    if (pos.x < -115.f)
        m_Dead = true;

    m_pGameInstance->Set_Active(TEXT("Effect_SemiclonDust"), &pos);
}

void CSemicolon::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CSemicolon::Render()
{

    if (FAILED(m_pColliderCom->Render()))
        return E_FAIL;

    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(0))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    Begin_RenderState();
    m_pVIBufferCom->Render();
    End_RenderState();
    return S_OK;
}

HRESULT CSemicolon::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Semiclon"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC Transform_desc;
    Transform_desc.fSpeedPerSec = 2.f;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_desc)))
        return E_FAIL;

    CCollider::COLLIDER_DESC  ColliderDesc{};
    ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vColliderScale = _float3(2.f, 15.f, 6.f);
    ColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSemicolon::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}

HRESULT CSemicolon::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

CSemicolon* CSemicolon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSemicolon* pInstance = new CSemicolon(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CSemicolon"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CSemicolon* CSemicolon::Clone(void* pArg)
{
    CSemicolon* pInstance = new CSemicolon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSemicolon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSemicolon::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);
}
