#include "Observer_Manager.h"
#include "Observer.h"
#include "SubjectObject.h"

CObserver_Manager::CObserver_Manager()
{
}

HRESULT CObserver_Manager::Initialize(_uint iNumLevels)
{
    m_iNumLevels = iNumLevels;
    m_Observers = new map<const _wstring,class CObserver*>[iNumLevels];

    return S_OK;
}


void CObserver_Manager::Clear(_uint iLevelndex)
{
    if (iLevelndex >= m_iNumLevels)
        return;

    for (auto& Pair : m_Observers[iLevelndex])
    {
        //Subject들에게도 옵저버가 사라졌음을 알림
        Pair.second->Clear_Subjects();
        Safe_Release(Pair.second);
    }

    m_Observers[iLevelndex].clear();
}

HRESULT CObserver_Manager::Add_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, CObserver* pObserver)
{
    if (nullptr == pObserver ||
        nullptr != Find_Observer(iObserverLevelndex, strObserverTag))
        return E_FAIL;

    m_Observers[iObserverLevelndex].emplace(strObserverTag, pObserver);

    return S_OK;
}

HRESULT CObserver_Manager::Subscribe_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, CSubjectObject* pSubject)
{
    if (nullptr == pSubject ||
        iObserverLevelndex >= m_iNumLevels)
        return E_FAIL;

    CObserver* pObserver = Find_Observer(iObserverLevelndex, strObserverTag);

    if(nullptr == pObserver)
        return E_FAIL;
    
    pObserver->Add_Subjects(pSubject);
    pSubject->Add_Observer(pObserver);
    

    return S_OK;
}

CObserver* CObserver_Manager::Find_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag)
{
    if (iObserverLevelndex >= m_iNumLevels)
        return nullptr;

    auto Pair = m_Observers[iObserverLevelndex].find(strObserverTag);

    if (Pair != m_Observers[iObserverLevelndex].end())
        return Pair->second;

    return nullptr;
}

CObserver_Manager* CObserver_Manager::Create(_uint iNumLevels)
{
    CObserver_Manager* pInstance = new CObserver_Manager();
     if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX(TEXT("Created Failed : CObserver_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CObserver_Manager::Free()
{
    __super::Free();

    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_Observers[i])
        {
            Pair.second->Clear_Subjects();
            Safe_Release(Pair.second);
        }
        
        m_Observers[i].clear();
    }

    Safe_Delete_Array(m_Observers);
}
