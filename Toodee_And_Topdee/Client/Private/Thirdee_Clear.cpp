#include "Thirdee_Clear.h"

CThirdee_Clear::CThirdee_Clear()
{
}

HRESULT CThirdee_Clear::Initialize(PLAYERSTATE eState)
{
    m_eState = eState;

    return S_OK;
}

void CThirdee_Clear::Enter(CPlayer_Thirdee* pPlayer)
{
}

void CThirdee_Clear::HandleInput(CPlayer_Thirdee* pPlayer, _uint iInputData, _float fTimeDelta)
{
}

void CThirdee_Clear::Update(CPlayer_Thirdee* pPlayer, _float fTimeDelta)
{
    pPlayer->Clear(fTimeDelta);
}

void CThirdee_Clear::Exit(CPlayer_Thirdee* pPlayer)
{
}

CThirdee_Clear* CThirdee_Clear::Create(PLAYERSTATE eState)
{
    CThirdee_Clear* pInstance = new CThirdee_Clear();

    if (FAILED(pInstance->Initialize(eState)))
    {
        MSG_BOX(TEXT("Created Failed : CThirdee_Clear"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CThirdee_Clear::Free()
{
    __super::Free();
}
