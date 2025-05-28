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


_bool CCollider::GetOverlapAll(list<class CGameObject*>*& pList)
{
    if (m_eState == COLLIDERSTATE::NONE) return false;
    pList = &m_pOthers;
    return true;
}


CGameObject* CCollider::GetOverlapTarget()
{
    /*  충돌 가장 마지막에 들어 온 객체를 꺼내오기.
        빠른 투사체 테스트는 현재 진행할 수 없어서 테스트를 못해봄.
        만약 정확하지 않으면 GetOverlapAll에서 꺼내서 이름 비교하며 처리하기*/

    return (m_eState == COLLIDERSTATE::NONE) ? nullptr : m_pOthers.back() != nullptr ? m_pOthers.back() : nullptr;
}

const COLLIDER_DIR CCollider::DetectCollisionDirection(_float* distance) const
{
    if (m_eState == COLLIDERSTATE::NONE) COLLIDER_DIR::CD_END;

    _float4x4		ViewMatrix{};
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

    _float3  myPosition = m_pTransform->Get_State(STATE::POSITION);
    _float3  myScale = m_pTransform->Get_Scaled();

    CTransform* other = dynamic_cast<CTransform*>(m_pOthers.back()->Get_Component(TEXT("Com_Transform")));

    if (other == nullptr)
       return  COLLIDER_DIR::CD_END;
    
    _float3  otherPosition = other->Get_State(STATE::POSITION);
    _float3  otherScale = other->Get_Scaled();
    _float3  vDelta = otherPosition - myPosition;

    D3DXVec3TransformNormal(&vDelta, &vDelta, &ViewMatrix);

    _float absX = fabsf(vDelta.x);
    _float absY = fabsf(vDelta.y);
    _float absZ = fabsf(vDelta.z);

    if (absY > absX && absY > absZ)
    {
        if (vDelta.y > 0) {
            *distance = (otherScale.y + myScale.y) * 0.5f - vDelta.y;
            return COLLIDER_DIR::BOTTOM;
        }
        else
        {
            *distance = (otherScale.y + myScale.y) * 0.5f + vDelta.y;
            return COLLIDER_DIR::TOP;
        }
    }
    else if (absX > absZ)
    {
        if (vDelta.x > 0)
        {
            *distance = (otherScale.x + myScale.x) * 0.5f - vDelta.x;
            return COLLIDER_DIR::LEFT;
        }
        else
        {
            *distance = (otherScale.x + myScale.x) * 0.5f + vDelta.x;
            return COLLIDER_DIR::RIGHT;
        }
    }
    else
    {
        if (vDelta.z > 0)
        {            
            *distance = (otherScale.z + myScale.z) * 0.5f - vDelta.z;
            return COLLIDER_DIR::FRONT;

        }
        else
        {
            *distance = (otherScale.z + myScale.z) * 0.5f + vDelta.z;
            return COLLIDER_DIR::BACK;
        }
    }


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
    if (Find_Others(pGameObject) == nullptr)
    {
        m_pOthers.emplace_back(pGameObject);
        Safe_AddRef(m_pOthers.back());
    }
}

void CCollider::Remove_Other(CGameObject* pGameObject)
{
    auto iter = find(m_pOthers.begin(), m_pOthers.end(), pGameObject);
    if (iter != m_pOthers.end())
    {
        Safe_Release(*iter);
        m_pOthers.remove(*iter);
    }
}


CGameObject* CCollider::Find_Others(CGameObject* other)
{
    auto iter = find(m_pOthers.begin(), m_pOthers.end(), other);
    if (iter == m_pOthers.end()) return nullptr;

    return *iter;
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
