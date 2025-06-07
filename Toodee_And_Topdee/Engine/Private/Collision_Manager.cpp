
#include "Collision_Manager.h"
#include "Collider.h"
#include "Collision.h"
#include "Transform.h"
#include "GameObject.h"

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	m_pColliders = new map<class CCollider*, COLLISION_INFO>[iNumLevels];

	return S_OK;
}


HRESULT CCollision_Manager::Add_Collider(_uint iLevelIndex, COLLIDER_SHAPE etype, CCollider** ppCollider)
{
	if (nullptr == *ppCollider)
	{
		MSG_BOX(TEXT("Failed to Add_Collider : CCollision_Manager"));
		return E_FAIL;
	}

    COLLISION_INFO  temp;
	temp.eType = etype;
	temp.bPreState = false;

 	m_pColliders[iLevelIndex].emplace(*ppCollider, temp);

	Safe_AddRef(*ppCollider);

	return S_OK;
}

void CCollision_Manager::Delete_Collider(_uint iLevelIndex,  CCollider** ppCollider)
{
     if (iLevelIndex >= m_iNumLevels) return;
    
     auto iter = m_pColliders[iLevelIndex].find(*ppCollider);
	 if (iter != m_pColliders[iLevelIndex].end())
	 {
		 Safe_Release(*ppCollider);
		 m_pColliders[iLevelIndex].erase(iter);
	 }


}

void CCollision_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

    for (auto& eInfo : m_pColliders[iLevelIndex])
    {
        CCollider* pCollider = eInfo.first;
        Safe_Release(pCollider);
    }

 	m_pColliders[iLevelIndex].clear();
}

//void CCollision_Manager::Check_Deleted()
//{
//	for (size_t i = 0; i < m_iNumLevels; ++i)
//	{
// 		for (auto iter = m_pColliders[i].begin(); iter != m_pColliders[i].end(); )
//		{
//			if (iter->first != nullptr)
//			{
//                CCollider* pCollider =iter->first;
//                Safe_Release(pCollider);
//				iter = m_pColliders[i].erase(iter);
//			}
//			else
//				++iter;
//
//		}
//        m_pColliders[i].clear();
//	}
//}


void CCollision_Manager::Check_Collision(_uint iLevelIndex, CCollider* pCollider)
{
    auto myIter = m_pColliders[iLevelIndex].find(pCollider);

    if (myIter == m_pColliders[iLevelIndex].end() || !pCollider->Collision_IsActive())
        return;

    /* 콜라이더 상태 초기화 */
    //pCollider->Set_State(CCollider::COLLIDER_STATE::NONE);

    /* 콜라이더 정보 설정 */
    CCollider::COLLIDER_DESC myDesc{}, otherDesc{};
    COLLIDER_SHAPE myShape = pCollider->Reference_Collider_Info(myDesc);

    /* my OBB 정보 설정 */
    CCollision::OBB myOBB = {}, otherOBB = {};

    _float3     vState[3];

    /* 움직이는 오브젝트 */
    if (!myDesc.bIsFixed)
    {
        vState[0] = myDesc.pTransform->Get_State(STATE::RIGHT);
        vState[1] = myDesc.pTransform->Get_State(STATE::UP);
        vState[2] = myDesc.pTransform->Get_State(STATE::LOOK);

        myOBB.center = myDesc.pTransform->Get_State(STATE::POSITION);
        myOBB.halfSize = myDesc.vColliderScale * 0.5f;
        for (size_t j = 0; j < 3; ++j)
            D3DXVec3Normalize(&myOBB.axis[j], &vState[j]);
    }
    /* 고정된 오브젝트 */
    else
    {
        myOBB.center = myDesc.vColliderPosion;
        myOBB.halfSize = myDesc.vColliderScale * 0.5f;
        myOBB.axis[0] = _float3(1.f, 0.f, 0.f);
        myOBB.axis[1] = _float3(0.f, 1.f, 0.f);
        myOBB.axis[2] = _float3(0.f, 0.f, 1.f);

    }
    /* 충돌되는 오브젝트 저장 */
    vector<CGameObject*> pCollisionOthers;
    bool bNowCollisionState = false;

    /* 검사 시작 */
    for (auto otherIter = m_pColliders[iLevelIndex].begin(); otherIter != m_pColliders[iLevelIndex].end(); ++otherIter)
    {
        /* 자기자신 pass , 반복문 상대가 비어있으면 pass, 콜라이더 비활성 pass*/
        if (myIter == otherIter || otherIter->first == nullptr|| !otherIter->first->Collision_IsActive())
            continue;

        /* 같은 오브젝트에 붙어 있는 콜라이더는 Pass */
        if (myIter->first->Get_GameObject() == otherIter->first->Get_GameObject())
            continue;

        /*  Other OBB 정보 설정 */
        COLLIDER_SHAPE otherShape = otherIter->first->Reference_Collider_Info(otherDesc);
       
        if (!otherDesc.bIsFixed)
        {
            vState[0] = otherDesc.pTransform->Get_State(STATE::RIGHT);
            vState[1] = otherDesc.pTransform->Get_State(STATE::UP);
            vState[2] = otherDesc.pTransform->Get_State(STATE::LOOK);

            otherOBB.center = otherDesc.pTransform->Get_State(STATE::POSITION);
            otherOBB.halfSize = otherDesc.vColliderScale * 0.5f;
            for (size_t j = 0; j < 3; ++j)
                D3DXVec3Normalize(&otherOBB.axis[j], &vState[j]);
        }
        else
        {
            otherOBB.center = otherDesc.vColliderPosion;
            otherOBB.halfSize = otherDesc.vColliderScale * 0.5f;
            otherOBB.axis[0] = _float3(1.f, 0.f, 0.f);
            otherOBB.axis[1] = _float3(0.f, 1.f, 0.f);
            otherOBB.axis[2] = _float3(0.f, 0.f, 1.f);
        }

        /* 충돌 검사 */
        bool bColliding = false;

        if (myShape == COLLIDER_SHAPE::RECT && otherShape == COLLIDER_SHAPE::RECT)
            bColliding = CCollision::Collision_Rect_Rect(myOBB, otherOBB);
        else if (myShape == COLLIDER_SHAPE::RECT && otherShape == COLLIDER_SHAPE::CUBE)
            bColliding = CCollision::Collision_Rect_Cube(myOBB, otherOBB);
        else if (myShape == COLLIDER_SHAPE::CUBE && otherShape == COLLIDER_SHAPE::RECT)
            bColliding = CCollision::Collision_Rect_Cube(otherOBB, myOBB);
        else if (myShape == COLLIDER_SHAPE::CUBE && otherShape == COLLIDER_SHAPE::CUBE)
            bColliding = CCollision::Collision_Cube_Cube(myOBB, otherOBB);

        if (bColliding)
        {
            pCollisionOthers.emplace_back(otherDesc.pOwner);
            bNowCollisionState = true;
        }
    }

    /* 충돌 상태 업데이트 */
    if (!myIter->second.bPreState && bNowCollisionState)  pCollider->Set_State(CCollider::COLLIDER_STATE::ENTRY);
    else if (myIter->second.bPreState && bNowCollisionState)  pCollider->Set_State(CCollider::COLLIDER_STATE::STAY);
    else if (myIter->second.bPreState && !bNowCollisionState)  pCollider->Set_State(CCollider::COLLIDER_STATE::EXIT);
    else                                                pCollider->Set_State(CCollider::COLLIDER_STATE::NONE);
    
    myIter->second.bPreState = bNowCollisionState;

    pCollider->Remove_Others();
    pCollider->Add_Others(pCollisionOthers);

}

CCollision_Manager* CCollision_Manager::Create(_uint iNumLevels)
{
	CCollision_Manager* pInstance = new CCollision_Manager;
	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CCollision_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollision_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
        for (auto& eInfo : m_pColliders[i])
        {
            CCollider* pCollider = eInfo.first;
            Safe_Release(pCollider);
        }

		m_pColliders[i].clear();
	}

	Safe_Delete_Array(m_pColliders);
}

