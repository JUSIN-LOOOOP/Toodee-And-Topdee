#include "Thirdee_Idle.h"

CThirdee_Idle::CThirdee_Idle()
{
}

HRESULT CThirdee_Idle::Initialize(PLAYERSTATE eState)
{
    m_eState = eState;

    return S_OK;
}

void CThirdee_Idle::Enter(CPlayer_Thirdee* pPlayer)
{
}

void CThirdee_Idle::HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if (iInputData == 0)
        return;

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        pPlayer->EnterMove();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::MOVE)))
            MSG_BOX(TEXT("Failed Change State : MOVE"));
    }

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_Z)) != 0)
    {
        pPlayer->EnterAction();
        if(FAILED(pPlayer->Change_State(PLAYERSTATE::ACTION)))
            MSG_BOX(TEXT("Failed Change State : ACTION"));
    }

}

void CThirdee_Idle::Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta)
{
    pPlayer->Idle(fTimeDelta);
}

void CThirdee_Idle::Exit(CPlayer_Thirdee* pPlayer)
{
}

CThirdee_Idle* CThirdee_Idle::Create(PLAYERSTATE eState)
{
    CThirdee_Idle* pInstance = new CThirdee_Idle();

    if (FAILED(pInstance->Initialize(eState)))
    {
        MSG_BOX(TEXT("Created Failed : CThirdee_Idle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThirdee_Idle::Free()
{
    __super::Free();
}
