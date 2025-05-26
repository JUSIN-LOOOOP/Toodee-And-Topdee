#include "State_Action.h"

CState_Action::CState_Action()
{
}

HRESULT CState_Action::Initialize(void* pArg)
{
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.05f;

    return S_OK;
}

void CState_Action::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Action::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
    

    if(iInputData & ENUM_CLASS(KEYINPUT::KEY_Z))
    {
        pPlayer->Action();
    }

    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_X))
    {
    //     pPlayer->Change_State(PLAYERSTATE::STOP);
    }

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        pPlayer->Move(fTimeDelta);

        if (!pPlayer->InAction())
            pPlayer->Change_State(PLAYERSTATE::MOVE);
    }

    if (iInputData == 0)
    {
        if (!pPlayer->InAction())
            pPlayer->Change_State(PLAYERSTATE::IDLE);
    }

}

void CState_Action::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    UpdateAnim(fTimeDelta);

    if (pPlayer->CanClear())
    {
        pPlayer->Clear();
        pPlayer->Change_State(PLAYERSTATE::CLEAR);
        return;
    }

}

void CState_Action::UpdateAnim(_float fTimeDelta)
{
    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
        m_fAnimTime = 0.f;
    }
    else
        m_fAnimTime += fTimeDelta;
}

void CState_Action::Exit(CPlayer* pPlayer)
{
}


CState_Action* CState_Action::Create(void* pArg)
{
    CState_Action* pInstance = new CState_Action();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Created Failed : CState_Action"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Action::Free()
{
    __super::Free();
}

