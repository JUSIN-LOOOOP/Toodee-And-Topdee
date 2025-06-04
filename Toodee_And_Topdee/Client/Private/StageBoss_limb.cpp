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
    //m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), m_fIdleTurnDir * fTimeDelta);
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

    if (D3DXVec3Length(&Length) > 7.f)
    {
        //m_pTransformCom->Move_To(fPlayerPos, fTimeDelta);
        /*_float3 Lookvec = fPlayerPos;
        Lookvec.y = fMonsterPos.y;
        m_pTransformCom->Look_At(Lookvec);*/
        //플레이어가 있는 방향으로 회전한다.
        _float3 lookVec = m_pTransformCom->Get_State(STATE::LOOK);
        lookVec.y = 0;
        D3DXVec3Normalize(&lookVec, &lookVec);

        _float3 Length = fPlayerPos - fMonsterPos;
        Length.y = 0;
        D3DXVec3Normalize(&Length, &Length);

        _float3 cross = {};
        D3DXVec3Cross(&cross, &lookVec, &Length);
        if (cross.y > .1f)
            m_pTransformCom->Turn(_float3{ 0.f, 1.f, 0.f }, fTimeDelta * 3);
        else if (cross.y < - .1f)
            m_pTransformCom->Turn(_float3{ 0.f, 1.f, 0.f }, -fTimeDelta * 3);
    }
 
    if (m_pTransformCom->Get_State(STATE::POSITION).y < 7.f)
        m_pTransformCom->Go_Up(fTimeDelta);

    D3DXVec3Normalize(&Dirvec, &Length);
    if (D3DXVec3Dot(&Dirvec, &Upvec) > 0.85)
        m_eState = STAGEMONERSTATE::TURN;

    _float3 vec = m_pTransformCom->Get_State(STATE::RIGHT);
    _float3 vec1 = m_pTransformCom->Get_State(STATE::UP);
    _float3 vec2 = m_pTransformCom->Get_State(STATE::LOOK);
    return S_OK;
}

HRESULT CStageBoss_limb::Turn(_float fTimeDelta)
{
    static _float fAccTime = 0;

    fAccTime += fTimeDelta;

    if (fAccTime >= 1.f)
    {
        fAccTime = 0.f;
        m_eState = STAGEMONERSTATE::ATTACK;
    }
    else
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::UP), fTimeDelta * 20);

    _float3 vec = m_pTransformCom->Get_State(STATE::RIGHT);
    _float3 vec1 = m_pTransformCom->Get_State(STATE::UP);
    _float3 vec2 = m_pTransformCom->Get_State(STATE::LOOK);

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

    _float3 vec = m_pTransformCom->Get_State(STATE::RIGHT);
    _float3 vec1 = m_pTransformCom->Get_State(STATE::UP);
    _float3 vec2 = m_pTransformCom->Get_State(STATE::LOOK);

    return S_OK;
}

HRESULT CStageBoss_limb::ChangeView(_float fTimeDelta)
{
    static _float AccAngle = 0.f;
    _float fDelta = 80.f * fTimeDelta;

    if (AccAngle + fDelta >= 90)
    {
        fDelta = 90 - AccAngle;
        m_eState = (m_eViewMode == VIEWMODE::TOPDEE) ? STAGEMONERSTATE::CHASE : STAGEMONERSTATE::IDLE;
    }

    AccAngle += fDelta;

    m_pTransformCom->Turn({ 1.f, 0.f, 0.f }, D3DXToRadian(m_eViewMode == VIEWMODE::TOODEE ? fDelta : -fDelta));

    if (m_eState != STAGEMONERSTATE::VIEWTURN)
        AccAngle = 0.f;
       
    return S_OK;
}

void CStageBoss_limb::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
