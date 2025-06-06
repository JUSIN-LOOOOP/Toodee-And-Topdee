#include "FireBall.h"
#include "GameInstance.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CFireBall::CFireBall(const CFireBall& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CFireBall::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFireBall::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;
    m_fTargetPos = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Player_TooDee"), TEXT("Com_Transform"), 0))->Get_State(STATE::POSITION);
   
    m_pTransformCom->Set_State(STATE::POSITION, *(static_cast<_float3*>(pArg)));

    m_fAngle = atan2f(m_fTargetPos.z - m_pTransformCom->Get_State(STATE::POSITION).z,
        m_fTargetPos.x - m_pTransformCom->Get_State(STATE::POSITION).x);

    if (m_fTargetPos.z > m_pTransformCom->Get_State(STATE::POSITION).z)
        m_fAngle *= -1.f;

    _float3 Right = _float3(1.f, 0.f, 0.f);
    _float3 Up = _float3(0.f, 1.f, 0.f);
    _float4x4 matRotX;
    D3DXMatrixRotationAxis(&matRotX, &Right, D3DXToRadian(90.f));

    _float4x4 matRotZ;
    D3DXMatrixRotationAxis(&matRotZ, &Up, -m_fAngle + D3DXToRadian(180));

    _float4x4 matFinal = matRotX * matRotZ;

    m_pTransformCom->Set_Matrix(matFinal);
    m_pTransformCom->Scaling(10.f, 4.5f, 10.f);

    m_fAngle = (_float)m_fAngle * (180.f / 3.141592);

    return S_OK;
}

void CFireBall::Priority_Update(_float fTimeDelta)
{
}

void CFireBall::Update(_float fTimeDelta)
{
    uFrameDelay += fTimeDelta;
    _float3 Position = m_pTransformCom->Get_State(STATE::POSITION);

    if (m_eState == FIREBALLSTATE::CREATE)
    {
        _float3 scale = m_pTransformCom->Get_Scaled();
        m_pTransformCom->Scaling(scale.x + .30f, scale.y + .30f, scale.z + .30f);
        if (scale.x > 5)
            m_eState = FIREBALLSTATE::CHASE;
    }

    _float fX = Position.x + cosf((_float)m_fAngle * (3.141592 / 180.f)) * fTimeDelta * 10;
    _float fZ = Position.z + sinf((_float)m_fAngle * (3.141592 / 180.f)) * fTimeDelta * 10;

    m_pTransformCom->Set_State(STATE::POSITION, _float3{ fX, Position.y, fZ });

    if (uFrameDelay > .2f)
    {
        ++ m_iTextureIdx;
        if (m_iTextureIdx == 10)
            m_iTextureIdx = 0;
        uFrameDelay = 0;
    }

    /*m_pGameInstance->Check_Collision(m_pColliderCom);
    if (m_pColliderCom->OnCollisionStay())
        m_Dead = true;*/

    
    if (Position.x < - 24 || Position.x > 24)
        m_Dead = true;
    if(Position.z < -13 || Position.z > 13)
        m_Dead = true;
}

void CFireBall::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CFireBall::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(m_iTextureIdx))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    Begin_RenderState();
    m_pVIBufferCom->Render();
    End_RenderState();
    return S_OK;
}

HRESULT CFireBall::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_FireBall"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC Transform_desc;
    Transform_desc.fRotationPerSec = 0.1f;
    Transform_desc.fSpeedPerSec = 0.2f;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_desc)))
        return E_FAIL;

    /*CCollider::COLLIDER_DESC  ColliderDesc{};
    ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vColliderScale = _float3(2.0f, 4.0f, 2.0f);
    ColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Rect"),
        TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;*/

    return S_OK;
}

HRESULT CFireBall::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}

HRESULT CFireBall::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

CFireBall* CFireBall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFireBall* pInstance = new CFireBall(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CFireBall"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CFireBall* CFireBall::Clone(void* pArg)
{
    CFireBall* pInstance = new CFireBall(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CFireBall"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFireBall::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    //Safe_Release(m_pColliderCom);
}
