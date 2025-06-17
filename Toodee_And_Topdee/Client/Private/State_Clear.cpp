#include "State_Clear.h"

CState_Clear::CState_Clear()
{
}

HRESULT CState_Clear::Initialize(void* pArg)
{
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.05f;

    return S_OK;
}

void CState_Clear::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Clear::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
}

void CState_Clear::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    if (pPlayer->ClearAnimStart())
        UpdateAnim(fTimeDelta);
}

void CState_Clear::Exit(CPlayer* pPlayer)
{
}

void CState_Clear::UpdateAnim(_float fTimeDelta)
{
    if (m_iCurrentAnimCount >= m_iMaxAnimCount - 1)
        return;

    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount++;
        m_fAnimTime = 0.f;
    }
    else
        m_fAnimTime += fTimeDelta;
}

CState_Clear* CState_Clear::Create(void* pArg)
{
    CState_Clear* pInstance = new CState_Clear();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed Created : CState_Clear"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Clear::Free()
{
    __super::Free();
}
