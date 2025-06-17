#include "State_Move.h"
#include "GameInstance.h"

CState_Move::CState_Move()
{
}

HRESULT CState_Move::Initialize(void* pArg)
{
    m_pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(m_pGameInstance);
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.05f;
    m_fSoundDelay = 0.25f;
    m_fSoundTime = 0.f;
    return S_OK;
}

void CState_Move::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Move::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        pPlayer->Move(iInputData, fTimeDelta);
    }

    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_Z))
    {
        pPlayer->Action();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::ACTION)))
            MSG_BOX(TEXT("Failed Change State : ACTION"));
    }

    if (iInputData == 0)
    {
        pPlayer->Idle();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::IDLE)))
            MSG_BOX(TEXT("Failed Change State : IDLE"));
    }
}

void CState_Move::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    UpdateAnim(fTimeDelta);

    if (pPlayer->CanClear())
    {
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::CLEAR)))
            MSG_BOX(TEXT("Failed Change State : CLEAR"));
    }

    if (pPlayer->CanActive() == false)
    {
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::STOP)))
            MSG_BOX(TEXT("Failed Change State : STOP"));
    }


    if (m_fSoundTime + fTimeDelta > m_fSoundDelay)
    {
        m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
        m_pGameInstance->PlayAudio(TEXT("PlayerFootStep.wav"), CHANNELID::SOUND_PLAYER, 0.5f);

        m_fSoundTime = 0.f;
    }
    else
        m_fSoundTime += fTimeDelta;
}

void CState_Move::UpdateAnim(_float fTimeDelta)
{
    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
        m_fAnimTime = 0.f;
    }
    else
        m_fAnimTime += fTimeDelta;
}
void CState_Move::Exit(CPlayer* pPlayer)
{
}

CState_Move* CState_Move::Create(void* pArg)
{
    CState_Move* pInstance = new CState_Move();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Created Failed : CState_Move"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Move::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}


