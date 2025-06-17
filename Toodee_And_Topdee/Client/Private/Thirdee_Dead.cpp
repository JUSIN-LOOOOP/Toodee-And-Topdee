#include "Thirdee_Dead.h"
#include "GameInstance.h"


CThirdee_Dead::CThirdee_Dead()
{
}

HRESULT CThirdee_Dead::Initialize(PLAYERSTATE eState)
{
    m_eState = eState;

    m_pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(m_pGameInstance);
    return S_OK;
}

void CThirdee_Dead::Enter(CPlayer_Thirdee* pPlayer)
{
    m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
    m_pGameInstance->PlayAudio(TEXT("Restart.wav"), CHANNELID::SOUND_PLAYER, 0.8f);
    m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
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
    Safe_Release(m_pGameInstance);
}
