#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device{ pGraphic_Device }
    , m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& Prototype)
    : m_pGraphic_Device{ Prototype.m_pGraphic_Device }
    , m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGraphic_Device);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
    return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
    return S_OK;
}

BLOCK_INFO CGameObject::Get_Block_Info()
{
    return BLOCK_INFO();
}

CComponent* CGameObject::Get_Component(const _wstring& strComponentTag)
{
    auto    iter = m_Components.find(strComponentTag);
    if (iter == m_Components.end())
        return nullptr;

    return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
    if (nullptr != Get_Component(strComponentTag))
        return E_FAIL;

    CComponent* pComponent = dynamic_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pComponent)
        return E_FAIL;

    m_Components.emplace(strComponentTag, pComponent);

    *ppOut = pComponent;

    Safe_AddRef(pComponent);

    return S_OK;
}

CGameObject* CGameObject::Clone(void* pArg)
{
    return nullptr;
}

void CGameObject::Free()
{
    __super::Free();

    for (auto& Pair : m_Components)
        Safe_Release(Pair.second);
    m_Components.clear();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGraphic_Device);

}
