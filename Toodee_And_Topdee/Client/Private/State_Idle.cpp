#include "State_Idle.h"


CState_Idle::CState_Idle()
{
}

HRESULT CState_Idle::Initialize(void* pArg)
{
	PLAYERSTATE_DESC* pDesc = static_cast<PLAYERSTATE_DESC*>(pArg);
    if (pDesc == nullptr)
        return E_FAIL;

	m_eState = pDesc->eState;

	m_iMaxAnimCount = pDesc->iMaxAnimCount;
	m_fAnimDelay = 0.05f;


    return S_OK;
}

void CState_Idle::Enter(CPlayer* pPlayer)
{
    m_iCurrentAnimCount = 0;
    m_fAnimTime = 0.f;
}

void CState_Idle::HandleInput(CPlayer* pPlayer, _uint iInputData, _float fTimeDelta)
{
    if (iInputData == 0)
        return;

    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        pPlayer->Move(fTimeDelta);
        if(FAILED(pPlayer->Change_State(PLAYERSTATE::MOVE)))
            MSG_BOX(TEXT("Failed Change State : MOVE"));
    }


    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_Z))
    {
        pPlayer->Action();
        if(FAILED(pPlayer->Change_State(PLAYERSTATE::ACTION)))
            MSG_BOX(TEXT("Failed Change State : ACTION"));
    }

    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_X))
    {
        pPlayer->Stop();
    }

    //Test
    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_X)) && (iInputData & ENUM_CLASS(KEYINPUT::KEY_Z)))
    {
        pPlayer->Clear();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::CLEAR)))
            MSG_BOX(TEXT("Failed Change State : CLEAR"));
    }
}

void CState_Idle::Update(CPlayer* pPlayer, _float fTimeDelta)
{
    UpdateAnim(fTimeDelta);

    if (pPlayer->CanClear())
    {
        pPlayer->Clear();
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::CLEAR)))
            MSG_BOX(TEXT("Failed Change State : CLEAR"));
    }

    if(pPlayer->CanActive() == false)
    {
        if (FAILED(pPlayer->Change_State(PLAYERSTATE::STOP)))
            MSG_BOX(TEXT("Failed Change State : STOP"));
    }
}

void CState_Idle::UpdateAnim(_float fTimeDelta)
{
    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
        m_fAnimTime = 0.f;
        
    }
    else
        m_fAnimTime += fTimeDelta;
}

void CState_Idle::Exit(CPlayer* pPlayer)
{
}
CState_Idle* CState_Idle::Create(void* pArg)
{
    CState_Idle* pInstance = new CState_Idle();
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Created Failed : CState_Idle"));
        Safe_Release(pInstance);
    }

	return pInstance;
}

void CState_Idle::Free()
{
    __super::Free();
}
