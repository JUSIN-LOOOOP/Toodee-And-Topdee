#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent { pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
    : CComponent{ Prototype }
    , m_eShape { Prototype.m_eShape }
{
}

HRESULT CCollider::Initialize_Prototype(COLLIDER_SHAPE eType)
{
    m_eShape = eType;

    return S_OK;
}


HRESULT CCollider::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    COLLIDER_DESC* pDesc = static_cast <COLLIDER_DESC*>(pArg);

    m_pOwner = pDesc->pOwner;
    m_pTransform = pDesc->pTransform;

    Safe_AddRef(m_pTransform);
    return S_OK;
}

COLLIDER_SHAPE CCollider::Reference_Collider_Info(CTransform** pTransform, CGameObject** pGameObject)
{
    *pTransform = m_pTransform;
    *pGameObject = m_pOwner;

    return m_eShape;
}

void CCollider::Add_Other(CGameObject* pGameObject)
{
    if (nullptr == pGameObject)
        return;
    
    m_pOthers.emplace_back(pGameObject);
    Safe_AddRef(m_pOthers.back());
}

void CCollider::Remove_Other(CGameObject* pGameObject)
{
    auto iter = find(m_pOthers.begin(), m_pOthers.end(), pGameObject);
    if (iter != m_pOthers.end())
    {
        Safe_Release(*iter);
        m_pOthers.erase(iter);
    }
}


CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, COLLIDER_SHAPE eType)
{
    CCollider* pInstance = new CCollider(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype(eType)))
    {
        MSG_BOX(TEXT("Failed to Created : CCollider"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CCollider* CCollider::Clone(void* pArg)
{
    CCollider* pInstance = new CCollider(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CCollider"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CCollider::Free()
{
    __super::Free();

    for (auto& pGameObject : m_pOthers)
        Safe_Release(pGameObject);
    m_pOthers.clear();

    Safe_Release(m_pTransform);

}
