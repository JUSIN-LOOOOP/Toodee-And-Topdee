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

void CStageBoss_limb::Update(_float fTimeDelta)
{
   m_fIdleTurnTime += fTimeDelta;

    if (m_fIdleTurnTime >= 1.f)
    {
        m_fIdleTurnDir *= - 1.f;
        m_fIdleTurnTime = 0;
    }
    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), m_fIdleTurnDir * -fTimeDelta);
}

void CStageBoss_limb::Late_Update(_float fTimeDelta)
{
}

HRESULT CStageBoss_limb::Chase(_float fTimeDelta)
{
    _float3 fPlayerPos = dynamic_cast<CTransform*>(m_pTopDee)->Get_State(Engine::STATE::POSITION);
    _float3 fMonsterPos = m_pTransformCom->Get_State(Engine::STATE::POSITION);
    _float3 Length = fMonsterPos - fPlayerPos;
    _float3 Dirvec;
    _float3 Upvec = { 0.f, 1.f, 0.f };
    _float3 LocalUpvec = m_pTransformCom->Get_State(Engine::STATE::UP);

    if (D3DXVec3Length(&Length) > 7.f)
    {
        m_pTransformCom->Move_To(fPlayerPos, fTimeDelta);
        
        _float4x4 RotMatrix = {};

        _float m_fAngle = atan2f(fPlayerPos.z - m_pTransformCom->Get_State(STATE::POSITION).z,
            fPlayerPos.x - m_pTransformCom->Get_State(STATE::POSITION).x);
        D3DXMatrixRotationAxis(&RotMatrix, &Upvec, -m_fAngle + D3DXToRadian(45));

        _float3 vRight = { 1.f, 0.f, 0.f };
        _float3 vLook = { 0.f, 0.f, 1.f };
        _float3 vUp = { 0.f, 1.f, 0.f };

        D3DXVec3TransformNormal(&vRight, &vRight, &RotMatrix);
        D3DXVec3TransformNormal(&vUp, &vUp, &RotMatrix);
        D3DXVec3TransformNormal(&vLook, &vLook, &RotMatrix);

        _float3 scale = m_pTransformCom->Get_Scaled();

        m_pTransformCom->Set_State(STATE::RIGHT, vRight * scale.x);
        m_pTransformCom->Set_State(STATE::UP, vUp * scale.y);
        m_pTransformCom->Set_State(STATE::LOOK, vLook * scale.z);
    }
 
    if (m_pTransformCom->Get_State(STATE::POSITION).y < 7.f)
        m_pTransformCom->Go_Up(fTimeDelta);

    D3DXVec3Normalize(&Dirvec, &Length);
    if (D3DXVec3Dot(&Dirvec, &Upvec) > 0.85)
        m_eState = STAGEMONERSTATE::TURN;

    return S_OK;
}

HRESULT CStageBoss_limb::Turn(_float fTimeDelta)
{
    static _float AccTime = 0.f;

    AccTime += fTimeDelta;

    if (AccTime >= 1.f)
    {
        AccTime = 0.f;
        m_eState = STAGEMONERSTATE::ATTACK;
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
        pos.y = 0;
        m_pTransformCom->Set_State(Engine::STATE::POSITION, pos);
        m_eState = STAGEMONERSTATE::CHASE;
    }

    return S_OK;
}

HRESULT CStageBoss_limb::ChangeView(_float fTimeDelta)
{
    static _float AccAngle = 0.f;
    _float fDelta = 80.f * fTimeDelta;
    _float3 Rightvec = { 1.f, 0.f, 0.f };
    _float3 WorldLookvec = { 0.f, 0.f, 1.f };

    if (AccAngle + fDelta > 125)
    {
        fDelta = 125 - AccAngle;
        m_eState = (m_eViewMode == VIEWMODE::TOPDEE) ? STAGEMONERSTATE::CHASE : STAGEMONERSTATE::IDLE;
    }

    AccAngle += fDelta;

    if (m_eViewMode == VIEWMODE::TOODEE)
        m_pTransformCom->Turn(Rightvec, D3DXToRadian(fDelta));
    else
        m_pTransformCom->Turn(Rightvec, -D3DXToRadian(fDelta));

    _float3 Lookvec = m_pTransformCom->Get_State(STATE::LOOK);
    _float4x4 rotationMatrix{};
    D3DXMatrixRotationAxis(&rotationMatrix, &WorldLookvec, -D3DXToRadian(45));
    _float3 FixedWorldRight;
    D3DXVec3TransformNormal(&FixedWorldRight, &Rightvec, &rotationMatrix);
    (&Lookvec, &Lookvec);
    D3DXVec3Normalize(&FixedWorldRight, &FixedWorldRight);
    float dot = D3DXVec3Dot(&Lookvec, &FixedWorldRight);

    if (m_eTurnFlag == true && fabs(dot) > 0.1f)
    {
        if (dot <= 0)
            m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), -D3DXToRadian(fDelta * 2.f));
        else
            m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), D3DXToRadian(fDelta * 2.f));
    }
    else
        m_eTurnFlag = false;

    if (m_eViewMode == VIEWMODE::TOODEE)
    {
        _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
        if (pos.y > 4.f)
        {
            pos.y -= 1.f;
            m_pTransformCom->Move_To(pos, fTimeDelta);
        }
    }
    
    if (m_eState != STAGEMONERSTATE::VIEWTURN)
    {
        AccAngle = 0.f;
        m_eTurnFlag = true;
    }

    return S_OK;
}

void CStageBoss_limb::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
