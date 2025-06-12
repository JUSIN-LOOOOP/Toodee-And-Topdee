#include "Thirdee_Dead.h"

CThirdee_Dead::CThirdee_Dead()
{
}

HRESULT CThirdee_Dead::Initialize(PLAYERSTATE eState)
{
    m_eState = eState;

    return S_OK;
}

void CThirdee_Dead::Enter(CPlayer_Thirdee* pPlayer)
{
}

void CThirdee_Dead::HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta)
{

}

void CThirdee_Dead::Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta)
{
    pPlayer->Dead(fTimeDelta);
}

void CThirdee_Dead::Exit(CPlayer_Thirdee* pPlayer)
{
}

CThirdee_Dead* CThirdee_Dead::Create(PLAYERSTATE eState)
{
    CThirdee_Dead* pInstance = new CThirdee_Dead();

    if (FAILED(pInstance->Initialize(eState)))
    {
        MSG_BOX(TEXT("Created Failed : CThirdee_Dead"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThirdee_Dead::Free()
{
    __super::Free();
}
