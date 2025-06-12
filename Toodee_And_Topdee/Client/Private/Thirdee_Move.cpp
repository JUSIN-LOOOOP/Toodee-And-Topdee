#include "Thirdee_Move.h"

CThirdee_Move::CThirdee_Move()
{
}

HRESULT CThirdee_Move::Initialize(PLAYERSTATE eState)
{
    m_eState = eState;

    return S_OK;
}

void CThirdee_Move::Enter(CPlayer_Thirdee* pPlayer)
{
}

void CThirdee_Move::HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if (iInputData == 0)
    {
        pPlayer->EnterIdle();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::IDLE)))
            MSG_BOX(TEXT("Failed Change State : IDLE"));
    }

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_Z)) != 0)
    {
        pPlayer->EnterAction();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::ACTION)))
            MSG_BOX(TEXT("Failed Change State : ACTION"));
    }

}

void CThirdee_Move::Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta)
{
    pPlayer->Move(fTimeDelta);
}

void CThirdee_Move::Exit(CPlayer_Thirdee* pPlayer)
{
}

CThirdee_Move* CThirdee_Move::Create(PLAYERSTATE eState)
{
    CThirdee_Move* pInstance = new CThirdee_Move();

    if (FAILED(pInstance->Initialize(eState)))
    {
        MSG_BOX(TEXT("Created Failed : CThirdee_Move"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThirdee_Move::Free()
{
    __super::Free();
}
