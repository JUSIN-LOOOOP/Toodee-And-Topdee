#include "State_Swim.h"
#include "Player_Toodee.h"
#include "GameInstance.h"

CState_Swim::CState_Swim()
{
}

HRESULT CState_Swim::Initialize(void* pArg)
{
    m_pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(m_pGameInstance);
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.05f;

	return S_OK;
}

void CState_Swim::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;

    m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
    m_pGameInstance->PlayAudio(TEXT("InWater.wav"), CHANNELID::SOUND_EFFECT, 0.3f);
    CPlayer_Toodee* pToodee = dynamic_cast<CPlayer_Toodee*>(pPlayer);

    pToodee->SwimEffect();

}

void CState_Swim::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        CPlayer_Toodee* pToodee = dynamic_cast<CPlayer_Toodee*>(pPlayer);

        pToodee->Swim(iInputData, fTimeDelta);
    }
}

void CState_Swim::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    UpdateAnim(fTimeDelta);

    if (pPlayer->CanActive() == false)
    {
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::STOP)))
            MSG_BOX(TEXT("Failed Change State : STOP"));
    }

    CPlayer_Toodee* pToodee = dynamic_cast<CPlayer_Toodee*>(pPlayer);

    if(false == pToodee->InWater())
    {
        pToodee->OutWater();
        pPlayer->Action();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::ACTION)))
            MSG_BOX(TEXT("Failed Change State : ACTION"));
    }
}

void CState_Swim::Exit(CPlayer* pPlayer)
{
}

void CState_Swim::UpdateAnim(_float fTimeDelta)
{
    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
        m_fAnimTime = 0.f;

    }
    else
        m_fAnimTime += fTimeDelta;
}

CState_Swim* CState_Swim::Create(void* pArg)
{
    CState_Swim* pInstance = new CState_Swim();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Created Failed : CState_Swim"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Swim::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}
