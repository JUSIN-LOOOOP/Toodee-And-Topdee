#include "FinalBoss.h"
#include "GameInstance.h"


CFinalBoss::CFinalBoss(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CFinalBoss::CFinalBoss(const CFinalBoss& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CFinalBoss::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFinalBoss::Initialize(void* pArg)
{
    name = TEXT("Final_Boss");

    if (FAILED(Ready_Components()))
        return E_FAIL;

    Ready_SubscribeEvent(m_pGameInstance->Get_NextLevelID());
    return S_OK;
}

void CFinalBoss::Priority_Update(_float fTimeDelta)
{
}

void CFinalBoss::Update(_float fTimeDelta)
{
    Motion_body(fTimeDelta);
    Motion_Attack(fTimeDelta);
    Motion_Effect(fTimeDelta);

}

void CFinalBoss::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CFinalBoss::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (!m_IsAttacking)
    {
        if (FAILED(m_pTextureCom_Idle->Bind_Texture(0)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTextureCom_Attack->Bind_Texture(m_iAttackNum)))
            return E_FAIL;
    }
    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();

    if (m_IsAttackEffect) { 
        SetUp_RenderState();

        m_pTransformCom->Scaling(1.f, 1.f, 1.f);
        m_pTransformCom->Set_State(STATE::POSITION, m_vAttackEffectPositionOffset);
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pTextureCom_Effect->Bind_Texture(m_iEffectNum)))
            return E_FAIL;
        m_pVIBufferCom->Bind_Buffers();
        m_pVIBufferCom->Render();

        m_pTransformCom->Scaling(0.3f, 0.3f, 0.3f);
        m_pTransformCom->Set_State(STATE::POSITION, m_vEffectProjectilePosition);
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pTextureCom_Projectile->Bind_Texture(0)))
            return E_FAIL;
        m_pVIBufferCom->Bind_Buffers();
        m_pVIBufferCom->Render();

        Reset_RenderState();
    }

    return S_OK;
}

void CFinalBoss::Event_FinalBoss_Attack(const FIANLBOSSATTACK_EVENT& event)
{
    m_IsAttacking = event.bIsAttacking;
    if (m_IsAttacking) {
        m_IsAttackEffect = true;
        m_vAttackShakingPosition = { 0.f,0.f,0.f };
        m_fEffectAccumurateTime = 0.f;
    }
    else
    {
        m_fAttackAccumurateTime = 0.f;
    }
}


HRESULT CFinalBoss::Ready_SubscribeEvent(_uint iPlayerLevel)
{
    m_pGameInstance->Subscribe<FIANLBOSSATTACK_EVENT>(iPlayerLevel, EVENT_KEY::FINALBOSS_ATTACK, [this](const FIANLBOSSATTACK_EVENT& Event) {
        this->Event_FinalBoss_Attack(Event); });

    return S_OK;
}


void CFinalBoss::Motion_body(_float fTimeDelta)
{
    if (m_fBodyIntervalTime <= m_fBodyAccumurateTime + fTimeDelta)
    {
        m_fBodyAccumurateTime = 0.f;
        m_bBodyTurn = !m_bBodyTurn;
    }
    else
        m_fBodyAccumurateTime += fTimeDelta;

    _float4x4 ViewMatrix{};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    _float3 vRight = { ViewMatrix._11, ViewMatrix._12, ViewMatrix._13 };
    _float3 vUp = { ViewMatrix._21, ViewMatrix._22, ViewMatrix._23 };
    _float3 vLook = { ViewMatrix._31, ViewMatrix._32, ViewMatrix._33 };
    _float3 vPos = { ViewMatrix._41, ViewMatrix._42, ViewMatrix._43 };

    D3DXVec3Normalize(&vLook, &vLook);

    m_vUpOffest += (m_bBodyTurn ? fTimeDelta : -fTimeDelta);
    _float3 vNextPos = vPos+ vRight * (2.f) + vLook * 8.f + vUp * m_vUpOffest;

    m_pTransformCom->Set_State(STATE::RIGHT, vRight * 10.f);
    m_pTransformCom->Set_State(STATE::UP, vUp * 10.f);
    m_pTransformCom->Set_State(STATE::LOOK, vLook);
    m_pTransformCom->Set_State(STATE::POSITION, vNextPos);

}

void CFinalBoss::Motion_Attack(_float fTimeDelta)
{
    if (!m_IsAttacking) return;

    if (m_fAttackIntervalTime <= m_fAttackAccumurateTime + fTimeDelta)
    {
        m_fAttackAccumurateTime = 0.f;
        if (m_bAttackTurn)
        {
            ++m_iAttackNum;
            if (m_iAttackNum >= 10)m_bAttackTurn = !m_bAttackTurn;
        }
        else
        {
            --m_iAttackNum;
            if (m_iAttackNum <= 0)m_bAttackTurn = !m_bAttackTurn;
        }
    }
    else
        m_fAttackAccumurateTime += fTimeDelta;


    if (m_fAttackShakingIntervalTime <= m_fAttackShakingAccumurateTime + fTimeDelta)
    {
        m_fAttackShakingAccumurateTime = 0.f;
        m_bAttackShakingTurn = !m_bAttackShakingTurn;
    }
    else
        m_fAttackShakingAccumurateTime += fTimeDelta;


    _float3 vOut{};
    if (m_bAttackShakingTurn)
        EaseVector3OutBounce(&vOut, m_vAttackShakingPosition, m_vAttackShakingOffestPosition, m_fAttackShakingAccumurateTime, m_fAttackShakingIntervalTime);
    else
        EaseVector3OutBounce(&vOut, m_vAttackShakingPosition, -m_vAttackShakingOffestPosition, m_fAttackShakingAccumurateTime, m_fAttackShakingIntervalTime);

    _float3 vPos = m_pTransformCom->Get_State(STATE::POSITION) + vOut - _float3(0.f, 1.5f, 0.f);
    m_pTransformCom->Set_State(STATE::POSITION, vPos);
}

void CFinalBoss::Motion_Effect(_float fTimeDelta)
{
    if (!m_IsAttackEffect) return;

    if (m_fEffectIntervalTime <= m_fEffectAccumurateTime + fTimeDelta)
    {
        m_fEffectAccumurateTime = 0.f;
        ++m_iEffectNum;
        if (m_iEffectNum == 5) {
            m_iEffectNum = 0;
            m_IsAttackEffect = false;
        }
    }
    else
        m_fEffectAccumurateTime += fTimeDelta;


    m_vAttackEffectPositionOffset = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vRight = m_pTransformCom->Get_State(STATE::RIGHT);
    _float3 vUp= m_pTransformCom->Get_State(STATE::UP);
    _float fSizeX = D3DXVec3Length(&vRight);
    _float fSizeY = D3DXVec3Length(&vUp);
    D3DXVec3Normalize(&vRight, &vRight);
    D3DXVec3Normalize(&vUp, &vUp);

    m_vAttackEffectPositionOffset += vRight* (fSizeX * (150.f / 512.f) - fSizeX * 0.5f)+ vUp* (-fSizeY * (45.f / 512.f) + fSizeY * 0.5f );

    if (m_iEffectNum == 1) {
        m_vEffectProjectilePosition = m_vAttackEffectPositionOffset;
        m_fEffectProjectilePositionX = m_vEffectProjectilePosition.x;
    }
    m_vEffectProjectilePosition += vUp * fTimeDelta* 10.f;
    m_vEffectProjectilePosition.x = m_fEffectProjectilePositionX;
}

HRESULT CFinalBoss::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture 1*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodoo"),
        TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;

    /* For.Com_Texture 11*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_FinalBoss_Attack"),
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Attack))))
        return E_FAIL;

    /* For.Com_Texture 5*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_FinalBoss_Attack"),
        TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Effect))))
        return E_FAIL;

    /* For.Com_Texture 1*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_FinalBoss_Attack_Projectile"),
        TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTextureCom_Projectile))))
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

void CFinalBoss::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

void CFinalBoss::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

CFinalBoss* CFinalBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFinalBoss* pInstance = new CFinalBoss(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CFinalBoss"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFinalBoss::Clone(void* pArg)
{
    CFinalBoss* pInstance = new CFinalBoss(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CFinalBoss"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFinalBoss::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom_Projectile);
    Safe_Release(m_pTextureCom_Effect);
    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Attack);
}
