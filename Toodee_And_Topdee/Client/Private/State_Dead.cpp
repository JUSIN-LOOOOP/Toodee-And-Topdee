#include "State_Dead.h"

CState_Dead::CState_Dead()
{
}

HRESULT CState_Dead::Initialize(void* pArg)
{
    PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_eState = pDesc->eState;

    m_iMaxAnimCount = pDesc->iMaxAnimCount;
    m_fAnimDelay = 0.05f;

    return S_OK;
}

void CState_Dead::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Dead::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
}

void CState_Dead::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    if (m_iCurrentAnimCount >= m_iMaxAnimCount -1 )
    {
    }
    else
        UpdateAnim(fTimeDelta);
}

void CState_Dead::Exit(CPlayer* pPlayer)
{
}

void CState_Dead::UpdateAnim(_float fTimeDelta)
{

    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount++;
        m_fAnimTime = 0.f;
    }
    else
        m_fAnimTime += fTimeDelta;
}

CState_Dead* CState_Dead::Create(void* pArg)
{
    CState_Dead* pInstance = new CState_Dead();

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed Created : CState_Dead"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Dead::Free()
{
    __super::Free();
}
