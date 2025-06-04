#include "Level_Manager.h"
#include "GameInstance.h"
#include "Level.h"

CLevel_Manager::CLevel_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelID, CLevel* pNewLevel)
{
    if (iLevelID != 1)m_pGameInstance->Set_CurrentLevelID(iLevelID);
    //기존 자원 파괴
    if (FAILED(Clear_Resources()))
        return E_FAIL;

    //현재 레벨이 어디선가 참조하고 있는지 검사
    if( 0 != Safe_Release(m_pCurrentLevel))
        MSG_BOX(TEXT("Failed to Change Level"));

    m_iCurrentLevelID = iLevelID;
    m_pCurrentLevel = pNewLevel;

    return S_OK;
}

void CLevel_Manager::Update(_float fTimeDelta)
{
    if (m_pCurrentLevel == nullptr)
        return;

    m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
    if (m_pCurrentLevel == nullptr)
        return E_FAIL;

    return m_pCurrentLevel->Render();
}

HRESULT CLevel_Manager::Clear_Resources()
{
    if (nullptr != m_pCurrentLevel)
        m_pGameInstance->Clear_Resources(m_iCurrentLevelID);

    return S_OK;
}

CLevel_Manager* CLevel_Manager::Create()
{
    return new CLevel_Manager;
}

void CLevel_Manager::Free()
{
    __super::Free();
    
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pCurrentLevel);

}
