#include "Thirdee_Action.h"

CThirdee_Action::CThirdee_Action()
{
}

HRESULT CThirdee_Action::Initialize(PLAYERSTATE eState)
{
    m_eState = eState;

    return S_OK;
}

void CThirdee_Action::Enter(CPlayer_Thirdee* pPlayer)
{
}

void CThirdee_Action::HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_Z)) != 0)
    {
        pPlayer->Action(fTimeDelta);
    }

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        pPlayer->Move(fTimeDelta);

        if (false == pPlayer->InAction())
        {
            pPlayer->EnterMove();
            if (FAILED(pPlayer->Change_State(PLAYERSTATE::MOVE)))
                MSG_BOX(TEXT("Failed Change State : MOVE"));
        }
    }


    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_Z)) == 0)
    {
        if (false == pPlayer->InAction())
        {
            pPlayer->EnterIdle();
            if (FAILED(pPlayer->Change_State(PLAYERSTATE::IDLE)))
                MSG_BOX(TEXT("Failed Change State : IDLE"));
        }
    }
}

void CThirdee_Action::Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta)
{
    if (false == pPlayer->InAction())
    {
        pPlayer->EnterIdle();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::IDLE)))
            MSG_BOX(TEXT("Failed Change State : IDLE"));
    }
}

void CThirdee_Action::Exit(CPlayer_Thirdee* pPlayer)
{
}

CThirdee_Action* CThirdee_Action::Create(PLAYERSTATE eState)
{
    CThirdee_Action* pInstance = new CThirdee_Action();

    if (FAILED(pInstance->Initialize(eState)))
    {
        MSG_BOX(TEXT("Created Failed : CThirdee_Action"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThirdee_Action::Free()
{
    __super::Free();
}
