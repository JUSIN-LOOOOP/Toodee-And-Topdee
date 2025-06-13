#include "Finger.h"
#include "GameInstance.h"

CFinger::CFinger(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CFinger::CFinger(const CFinger& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CFinger::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFinger::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_pTransformCom->Scaling(10.f, 10.f, 10.f);
    _float3 pos = *(static_cast<_float3*>(pArg));
    _float3 PlayerPos = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(m_pGameInstance->Get_CurrentLevelID(), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0))->Get_State(STATE::POSITION);
    pos.x = m_pGameInstance->Rand(PlayerPos.x, PlayerPos.x + 20.f);
    pos.y = 40.f;
    m_pTransformCom->Set_State(STATE::POSITION, pos);
    m_pTransformCom->Rotation(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(90));
    
    name = TEXT("EnemyFinger");

    return S_OK;
}

void CFinger::Priority_Update(_float fTimeDelta)
{
}

void CFinger::Update(_float fTimeDelta)
{
    m_pTransformCom->Go_Down(fTimeDelta * 12.f);

    if (m_pTransformCom->Get_State(STATE::POSITION).y < 4.f && m_bFallFlag == false)
    {
        m_bFallFlag = true;
        SHAKING Event;
        Event.fTime = .5f;
        m_pGameInstance->Publish(m_pGameInstance->Get_CurrentLevelID(), EVENT_KEY::CAM_SHAKING, Event);
        _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
        m_pGameInstance->Set_Active(TEXT("Effect_FingerDust"), &pos);
    }
    
    if (m_pTransformCom->Get_State(STATE::POSITION).y < -5.f)
        m_Dead = true;
}

void CFinger::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CFinger::Render()
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

HRESULT CFinger::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Finger"),
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
    ColliderDesc.vColliderScale = _float3(3.0f, 10.0f, 10.0f);
    ColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CFinger::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}

HRESULT CFinger::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

CFinger* CFinger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
      CFinger* pInstance = new CFinger(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CFinger"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CFinger* CFinger::Clone(void* pArg)
{
    CFinger* pInstance = new CFinger(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CFinger"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFinger::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);
}
