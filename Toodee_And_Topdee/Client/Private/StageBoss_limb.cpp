#include "StageBoss_limb.h"
#include "GameInstance.h"

CStageBoss_limb::CStageBoss_limb(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject(pGraphic_Device)
{
}

CStageBoss_limb::CStageBoss_limb(const CStageBoss_limb& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CStageBoss_limb::Initialize_Prototype()
{
    return S_OK;
}

void CStageBoss_limb::Priority_Update(_float fTimeDelta)
{

}

void CStageBoss_limb::Update(_float fTimeDelta)
{
    m_fIdleTurnTime += fTimeDelta;

    if (m_fIdleTurnTime >= .5f)
    {
        m_fIdleTurnDir *= -1.f;
        m_fIdleTurnTime = 0;
    }
    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), m_fIdleTurnDir * -fTimeDelta * 0.8);

    switch (m_eState)
    {
    case STAGEMONERSTATE::CHASE:
        Chase(fTimeDelta);
        break;
    case STAGEMONERSTATE::TURN:
        Turn(fTimeDelta);
        break;
    case STAGEMONERSTATE::HIT:
        HIT(fTimeDelta);
        break;
    case STAGEMONERSTATE::VIEWTURN:
        ChangeView(fTimeDelta);
        break;
    case STAGEMONERSTATE::DAMAGE:
        MoveToOrigin(fTimeDelta);
        break;
    }
}

void CStageBoss_limb::Late_Update(_float fTimeDelta)
{
}

HRESULT CStageBoss_limb::Chase(_float fTimeDelta)
{
    _float3 fPlayerPos = dynamic_cast<CTransform*>(m_pTopDee)->Get_State(Engine::STATE::POSITION);
    _float3 Length = m_pTransformCom->Get_State(Engine::STATE::POSITION) - fPlayerPos;
    _float3 Upvec = { 0.f, 1.f, 0.f };
    _bool   flag[2] = {};

    D3DXVec3Normalize(&Length, &Length);

    if (D3DXVec3Dot(&Length, &Upvec) < 0.95)
    {
        m_pTransformCom->Move_To(fPlayerPos, fTimeDelta * 3);

        _float4x4 RotMatrix = {};
        _float m_fAngle = atan2f(fPlayerPos.z - m_pTransformCom->Get_State(STATE::POSITION).z,
            fPlayerPos.x - m_pTransformCom->Get_State(STATE::POSITION).x);
        D3DXMatrixRotationY(&RotMatrix, -m_fAngle + D3DXToRadian(45));
        m_pTransformCom->Set_Matrix(RotMatrix);
        flag[0] = true;
    }

    if (m_pTransformCom->Get_State(STATE::POSITION).y < 7.f)
    {
        m_pTransformCom->Go_Up(fTimeDelta * 2.f);
        flag[1] = true;
    }
        
    if (flag[0] == false && flag[1] == false)
        m_eState = STAGEMONERSTATE::TURN;

    return S_OK;
}

HRESULT CStageBoss_limb::Turn(_float fTimeDelta)
{
    m_fTurnTime += fTimeDelta;

    if (m_fTurnTime >= 1.f)
    {
        m_fTurnTime = 0.f;
        m_eState = STAGEMONERSTATE::HIT;
    }
    else
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), fTimeDelta * 30.f);

    return S_OK;
}

HRESULT CStageBoss_limb::HIT(_float fTimeDelta)
{
    m_pTransformCom->Go_Down(fTimeDelta);

    _float3 pos = m_pTransformCom->Get_State(Engine::STATE::POSITION);
    if (pos.y <= 2.f)
    {
        pos.y = 2.f;
        m_pTransformCom->Set_State(Engine::STATE::POSITION, pos);
        m_eState = STAGEMONERSTATE::IDLE;
        MONSTERSIGNAL mode;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::FIN_ACTION, mode);
    }

    return S_OK;
}

HRESULT CStageBoss_limb::ChangeView(_float fTimeDelta)
{
    _float fDelta = 80.f * fTimeDelta;

    if (m_AccAngle + fDelta >= 120)
    {
        fDelta = 120 - m_AccAngle;
        m_eState = STAGEMONERSTATE::IDLE;
    }

    m_AccAngle += fDelta;

    if (m_AccAngle <= 120)
        m_pTransformCom->Turn({ 1.f, 0.f, 0.f }, D3DXToRadian(((m_eViewMode == VIEWMODE::TOODEE)) ? fDelta : -fDelta));

    if (m_eTurnFlag == true)
        RotateToFace(fDelta);

    if (m_eViewMode == VIEWMODE::TOODEE)
    {
        _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
        if (pos.y > 4.f)
        {
            pos.y -= 1.f;
            m_pTransformCom->Move_To(pos, fTimeDelta);
        }
    }

    if (m_eState == STAGEMONERSTATE::IDLE)
    {
        m_AccAngle = 0.f;
        m_eTurnFlag = true;
        MONSTERSIGNAL mode;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::FIN_ACTION, mode);
    }

    return S_OK;
}

HRESULT CStageBoss_limb::MoveToOrigin(_float fTimeDelta)
{
    _float3 Length = m_pTransformCom->Get_State(Engine::STATE::POSITION) - m_fInitPos;
    if (D3DXVec3Length(&Length) > 0.f || m_eState == STAGEMONERSTATE::DAMAGE)
    {
        m_pTransformCom->Move_To(m_fInitPos, fTimeDelta * 0.8f);
        ChangeView(fTimeDelta);
    }
    else
    {
        m_eState = STAGEMONERSTATE::IDLE;
        MONSTERSIGNAL mode;
        m_eTurnFlag = true;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::FIN_ACTION, mode);
    }
    return S_OK;
}

void CStageBoss_limb::RotateToFace(_float fTimeDelta)
{
    _float3 WorldLookvec = { 0.f, 0.f, 1.f };
    _float3 Rightvec = { 1.f, 0.f, 0.f };

    _float3 Lookvec = m_pTransformCom->Get_State(STATE::LOOK);
    _float4x4 rotationMatrix{};
    D3DXMatrixRotationAxis(&rotationMatrix, &WorldLookvec, -D3DXToRadian(45));
    _float3 FixedWorldRight;
    D3DXVec3TransformNormal(&FixedWorldRight, &Rightvec, &rotationMatrix);
    (&Lookvec, &Lookvec);
    D3DXVec3Normalize(&FixedWorldRight, &FixedWorldRight);
    float dot = D3DXVec3Dot(&Lookvec, &FixedWorldRight);

    if (fabs(dot) > 0.1f)
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), D3DXToRadian(((dot <= 0)) ? -fTimeDelta : fTimeDelta * 2.f));
    else
        m_eTurnFlag = false;
}

void CStageBoss_limb::GetSignal(const MONSTERSIGNAL& Event)
{
    m_eState = (STAGEMONERSTATE)Event.iState;
    m_eViewMode = (VIEWMODE)Event.iViewMode;
}

HRESULT CStageBoss_limb::Ready_SubscribeEvent(_uint iPlayerLevel)
{
    m_pGameInstance->Subscribe<MONSTERSIGNAL>(iPlayerLevel, EVENT_KEY::SIG_MONSTER, [this](const MONSTERSIGNAL& Event) {
        this->GetSignal(Event); });

    return S_OK;
}

void CStageBoss_limb::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
