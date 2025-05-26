#include "State_Stop.h"

CState_Stop::CState_Stop()
{
}

HRESULT CState_Stop::Initialize(void* pArg)
{
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.05f;

    return S_OK;
}

void CState_Stop::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Stop::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
}

void CState_Stop::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    if(pPlayer->CanActive())
    {
        if (FAILED(pPlayer->Return_PrevState()))
            MSG_BOX(TEXT("Failed Return PrevState"));
    }
}

void CState_Stop::Exit(CPlayer* pPlayer)
{
}

void CState_Stop::UpdateAnim(_float fTimeDelta)
{
}

CState_Stop* CState_Stop::Create(void* pArg)
{
    CState_Stop* pInstance = new CState_Stop();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Created Failed : CState_Stop"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Stop::Free()
{
}
