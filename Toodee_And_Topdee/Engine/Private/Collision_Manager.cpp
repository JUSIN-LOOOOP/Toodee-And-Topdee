
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
	m_pColliders = new multimap<COLLIDER_SHAPE, COLLIDER_STATE_INFO>[iNumLevels];

	return S_OK;
}

void CCollision_Manager::Update(_float fTimeDelta)
{
	//Check_Deleted();
	Check_Collision();
}


HRESULT CCollision_Manager::Add_Collider(_uint iLevelIndex, COLLIDER_SHAPE etype, CCollider** pCollider)
{
	if (nullptr == *pCollider)
	{
		MSG_BOX(TEXT("Failed to Add_Collider : CCollision_Manager"));
		return E_FAIL;
	}

	COLLIDER_STATE_INFO  temp;
	temp.pCollider = *pCollider;
	temp.bCurState = false;
	temp.bPreState = false;

 	m_pColliders[iLevelIndex].emplace(etype, temp);

	Safe_AddRef(temp.pCollider);

	return S_OK;
}

void CCollision_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& eInfo : m_pColliders[iLevelIndex])
		Safe_Release(eInfo.second.pCollider);

	m_pColliders[iLevelIndex].clear();
}

void CCollision_Manager::Check_Deleted()
{
	for (size_t i = 0; i < m_iNumLevels; ++i)
	{
		for (auto iter = m_pColliders[i].begin(); iter != m_pColliders[i].end(); )
		{
			if (iter->second.pCollider == nullptr)
			{
				Safe_Release(iter->second.pCollider);
				iter = m_pColliders[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}


void CCollision_Manager::Check_Collision()
{
    for (size_t i = 0; i < m_iNumLevels; ++i)
    {
        // 모든 콜라이더의 상태를 먼저 NONE으로 초기화
        for (auto& pair : m_pColliders[i])
        {
            if (pair.second.pCollider->Collider_IsActive())
            {
                pair.second.pCollider->Set_State(CCollider::COLLIDERSTATE::NONE);
            }
        }

        // 충돌 검사
        for (auto iter_A = m_pColliders[i].begin(); iter_A != m_pColliders[i].end(); ++iter_A)
        {
            COLLIDER_SHAPE eType_A = iter_A->first;
            COLLIDER_STATE_INFO& info_A = iter_A->second;

            if (!info_A.pCollider->Collider_IsActive()) {
                continue;
            }

            CCollider::COLLIDER_DESC desc_A = {};
            info_A.pCollider->Reference_Collider_Info(&desc_A.pTransform, &desc_A.pOwner);

            // 현재 프레임에서 A가 충돌하는 객체들을 저장
            vector<CGameObject*> currentCollisions;
            bool hasAnyCollision = false;

            for (auto iter_B = m_pColliders[i].begin(); iter_B != m_pColliders[i].end(); ++iter_B)
            {
                if (iter_A == iter_B) continue;

                COLLIDER_SHAPE eType_B = iter_B->first;
                COLLIDER_STATE_INFO& info_B = iter_B->second;

                if (!info_B.pCollider->Collider_IsActive()) {
                    continue;
                }

                CCollider::COLLIDER_DESC desc_B = {};
                info_B.pCollider->Reference_Collider_Info(&desc_B.pTransform, &desc_B.pOwner);

                // 같은 객체끼리는 충돌 검사 안함
                if (desc_A.pOwner == desc_B.pOwner) {
                    continue;
                }

                /* OBB 정보 설정 */
                CCollision::OBB OBB_A = {}, OBB_B = {};
                _float3 vState[3];

                // OBB_A 설정
                vState[0] = desc_A.pTransform->Get_State(STATE::RIGHT);
                vState[1] = desc_A.pTransform->Get_State(STATE::UP);
                vState[2] = desc_A.pTransform->Get_State(STATE::LOOK);

                OBB_A.center = desc_A.pTransform->Get_State(STATE::POSITION);
                OBB_A.halfSize = desc_A.pTransform->Get_Scaled() * 0.5f;
                for (size_t j = 0; j < 3; ++j)
                    D3DXVec3Normalize(&OBB_A.axis[j], &vState[j]);

                // OBB_B 설정
                vState[0] = desc_B.pTransform->Get_State(STATE::RIGHT);
                vState[1] = desc_B.pTransform->Get_State(STATE::UP);
                vState[2] = desc_B.pTransform->Get_State(STATE::LOOK);

                OBB_B.center = desc_B.pTransform->Get_State(STATE::POSITION);
                OBB_B.halfSize = desc_B.pTransform->Get_Scaled() * 0.5f;
                for (size_t j = 0; j < 3; ++j)
                    D3DXVec3Normalize(&OBB_B.axis[j], &vState[j]);

                /* 충돌 검사 */
                bool bColliding = false;

                if (eType_A == COLLIDER_SHAPE::RECT && eType_B == COLLIDER_SHAPE::RECT)
                    bColliding = CCollision::Collision_Rect_Rect(OBB_A, OBB_B);
                else if (eType_A == COLLIDER_SHAPE::RECT && eType_B == COLLIDER_SHAPE::CUBE)
                    bColliding = CCollision::Collision_Rect_Cube(OBB_A, OBB_B);
                else if (eType_A == COLLIDER_SHAPE::CUBE && eType_B == COLLIDER_SHAPE::RECT)
                    bColliding = CCollision::Collision_Rect_Cube(OBB_B, OBB_A);
                else if (eType_A == COLLIDER_SHAPE::CUBE && eType_B == COLLIDER_SHAPE::CUBE)
                    bColliding = CCollision::Collision_Cube_Cube(OBB_A, OBB_B);

                if (bColliding)
                {
                    currentCollisions.emplace_back(desc_B.pOwner);
                    hasAnyCollision = true;
                }
            }

            // A 객체의 충돌 상태 결정 및 업데이트
            UpdateColliderState(info_A, desc_A.pOwner, currentCollisions, hasAnyCollision);
        }
    }
}

void CCollision_Manager::UpdateColliderState(COLLIDER_STATE_INFO& info, CGameObject* owner,
    const vector<CGameObject*>& currentCollisions, bool hasAnyCollision)
{
    // 이전 프레임 충돌 상태 확인
    bool hadPreviousCollision = info.bPreState;

    if (!hadPreviousCollision && hasAnyCollision)
    {
        // Entry: 새로운 충돌 시작
        info.pCollider->Set_State(CCollider::COLLIDERSTATE::ENTRY);

        // 새로운 충돌 객체들만 추가 (중복 방지)
        for (auto* obj : currentCollisions)
        {
            info.pCollider->Add_Other(obj);
        }
    }
    else if (hadPreviousCollision && hasAnyCollision)
    {
        // Stay: 계속 충돌 중
        info.pCollider->Set_State(CCollider::COLLIDERSTATE::STAY);

        // 충돌 리스트 업데이트 (이전 것들은 제거하고 새로 추가)
        // 이 부분에서 메모리 관리 주의
        list<CGameObject*> previousOthers;
        if (info.pCollider->GetOverlapAll_Copy(&previousOthers))
        {
            // 이전 충돌 객체들 제거
            for (auto* prevObj : previousOthers)
            {
                info.pCollider->Remove_Other(prevObj);
            }
        }

        // 현재 충돌 객체들 추가
        for (auto* obj : currentCollisions)
        {
            info.pCollider->Add_Other(obj);
        }
    }
    else if (hadPreviousCollision && !hasAnyCollision)
    {
        // Exit: 충돌 종료
        info.pCollider->Set_State(CCollider::COLLIDERSTATE::EXIT);

        // 모든 충돌 객체 제거
        list<CGameObject*> othersToRemove;
        if (info.pCollider->GetOverlapAll_Copy(&othersToRemove))
        {
            for (auto* obj : othersToRemove)
            {
                info.pCollider->Remove_Other(obj);
            }
        }
    }
    else
    {
        // None: 충돌 없음
        info.pCollider->Set_State(CCollider::COLLIDERSTATE::NONE);
    }

    // 상태 업데이트
    info.bPreState = hasAnyCollision;
    info.bCurState = hasAnyCollision;
}


//void CCollision_Manager::Check_Collision()
//{
//
//	for (size_t i = 0; i < m_iNumLevels; ++i)
//	{
//
//		for (auto iter_A = m_pColliders[i].begin(); iter_A != m_pColliders[i].end(); )
//		{
//			COLLIDER_SHAPE				eType_A = iter_A->first;
//			COLLIDER_STATE_INFO&		info_A = iter_A->second;
//
//			if (!info_A.pCollider->Collider_IsActive()) {
//				++iter_A;
//				continue;
//			}
//
//			CCollider::COLLIDER_DESC	desc_A = {};
//			
//			info_A.pCollider->Reference_Collider_Info(&desc_A.pTransform, &desc_A.pOwner);
//
//
//			for (auto iter_B = m_pColliders[i].begin(); iter_B != m_pColliders[i].end();)
//			{
//				if (iter_A == iter_B)
//				{
//					++iter_B;
//					continue;
//				}
//				
//
//				COLLIDER_SHAPE				eType_B = iter_B->first;
//				COLLIDER_STATE_INFO&		info_B = iter_B->second;
//
//				if (!info_B.pCollider->Collider_IsActive()) {
//					++iter_B;
//					continue;
//				}
//
//				CCollider::COLLIDER_DESC	desc_B = {};
//
//				info_B.pCollider->Reference_Collider_Info(&desc_B.pTransform, &desc_B.pOwner);
//
//
//				/* Collider 정보 읽기 */
//
//				CCollision::OBB OBB_A = {};
//				CCollision::OBB OBB_B = {};
//				
//				_float3 vState[3];
//
//				vState[0] = desc_A.pTransform->Get_State(STATE::RIGHT);
//				vState[1] = desc_A.pTransform->Get_State(STATE::UP);
//				vState[2] = desc_A.pTransform->Get_State(STATE::LOOK);
//
//				OBB_A.center = desc_A.pTransform->Get_State(STATE::POSITION);
//				OBB_A.halfSize = desc_A.pTransform->Get_Scaled() * 0.5;
//				for (size_t i = 0; i < 3; ++i) 
//					D3DXVec3Normalize(&OBB_A.axis[i], &vState[i]);
//
//				vState[0] = desc_B.pTransform->Get_State(STATE::RIGHT);
//				vState[1] = desc_B.pTransform->Get_State(STATE::UP);
//				vState[2] = desc_B.pTransform->Get_State(STATE::LOOK);
//
//				OBB_B.center = desc_B.pTransform->Get_State(STATE::POSITION);
//				OBB_B.halfSize = desc_B.pTransform->Get_Scaled() * 0.5;
//				for (size_t i = 0; i < 3; ++i)
//					D3DXVec3Normalize(&OBB_B.axis[i], &vState[i]);
//
//
//				/* 충돌 검사 시작 */
//
//				_bool bNowState = false;
//
//				if (eType_A == COLLIDER_SHAPE::RECT)
//					if (eType_B == COLLIDER_SHAPE::RECT)
//						bNowState = CCollision::Collision_Rect_Rect(OBB_A, OBB_B);
//					else
//						bNowState = CCollision::Collision_Rect_Cube(OBB_A, OBB_B);
//				else 
//					if (eType_B == COLLIDER_SHAPE::RECT)
//						bNowState = CCollision::Collision_Rect_Cube(OBB_B, OBB_A);
//					else
//						bNowState = CCollision::Collision_Cube_Cube(OBB_A, OBB_B);
//
//
//				/*충돌 검사 결과 */
//
//				info_A.bCurState = bNowState;
//
//				if (!info_A.bPreState && info_A.bCurState) // Entry
//				{
//					info_A.pCollider->Add_Other(desc_B.pOwner);
//					//info_B.pCollider->Add_Other(desc_A.pOwner);
//
//					info_A.pCollider->Set_State(CCollider::COLLIDERSTATE::ENTRY);
//					//info_B.pCollider->Set_State(CCollider::COLLIDERSTATE::ENTRY);
//				}
//				else if (info_A.bPreState && info_A.bCurState) //Stay
//				{
//					info_A.pCollider->Set_State(CCollider::COLLIDERSTATE::STAY);
//					//info_B.pCollider->Set_State(CCollider::COLLIDERSTATE::STAY);
//
//				}
//				else if (info_A.bPreState && !info_A.bCurState) //Exit
//				{
//					info_A.pCollider->Remove_Other(desc_B.pOwner);
//					//info_B.pCollider->Remove_Other(desc_A.pOwner);
//
//					info_A.pCollider->Set_State(CCollider::COLLIDERSTATE::EXIT);
//					//info_B.pCollider->Set_State(CCollider::COLLIDERSTATE::EXIT);
//				}
//				else											//None
//				{
//					info_A.pCollider->Set_State(CCollider::COLLIDERSTATE::NONE);
//					//info_B.pCollider->Set_State(CCollider::COLLIDERSTATE::NONE);
//				}
//
//				info_A.bPreState = info_A.bCurState;
//
//				++iter_B;
//			}
//
//			++iter_A;
//		}
//	}
//}
//

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
			Safe_Release(eInfo.second.pCollider);

		m_pColliders[i].clear();
	}

	Safe_Delete_Array(m_pColliders);

}

