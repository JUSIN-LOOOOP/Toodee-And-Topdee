#include "Collider.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collision.h"
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
    m_vScale = pDesc->vColliderScale;
    if (pDesc->bIsFixed)
    {   
        m_vPosition = pDesc->vColliderPosion;
        m_bIsFixed = pDesc->bIsFixed;
    }
    else
        m_vPosition = pDesc->pTransform->Get_State(STATE::POSITION);

    Safe_AddRef(m_pTransform);

    m_iNumVertices = 8;
    m_iVertexStride = sizeof(VTXCUBETEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
    m_ePrimitiveType = D3DPT_TRIANGLELIST;
    m_iNumPrimitive = 12;
    m_iIndexStride = 2;
    m_iNumIndices = 36;
    m_eIndexFormat = D3DFMT_INDEX16;


#pragma region VERTEX_BUFFER
    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
        return E_FAIL;

    VTXCUBETEX* pVertices = { nullptr };

    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    pVertices[0].vPosition = m_vPoint[0] = _float3(-m_vScale.x, m_vScale.y, -m_vScale.z)  *0.5f   + m_vPosition;
    pVertices[1].vPosition = m_vPoint[1] = _float3(m_vScale.x, m_vScale.y, -m_vScale.z)   *0.5f  + m_vPosition;
    pVertices[2].vPosition = m_vPoint[2] = _float3(m_vScale.x, -m_vScale.y, -m_vScale.z)  *0.5f  + m_vPosition;
    pVertices[3].vPosition = m_vPoint[3] = _float3(-m_vScale.x, -m_vScale.y, -m_vScale.z) *0.5f   + m_vPosition;
    pVertices[4].vPosition = m_vPoint[4] = _float3(-m_vScale.x, m_vScale.y, m_vScale.z)   *0.5f  + m_vPosition;
    pVertices[5].vPosition = m_vPoint[5] = _float3(m_vScale.x, m_vScale.y, m_vScale.z)    *0.5f  + m_vPosition;
    pVertices[6].vPosition = m_vPoint[6] = _float3(m_vScale.x, -m_vScale.y, m_vScale.z)   *0.5f  + m_vPosition;
    pVertices[7].vPosition = m_vPoint[7] = _float3(-m_vScale.x, -m_vScale.y, m_vScale.z)  *0.5f  + m_vPosition;

    m_pVB->Unlock();

#pragma endregion 


#pragma region INDEX_BUFFER

    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    _ushort* pIndices = { nullptr };

    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
    pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;
    pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
    pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;
    pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
    pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;
    pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
    pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;
    pIndices[24] = 5; pIndices[25] = 4; pIndices[26] = 7;
    pIndices[27] = 5; pIndices[28] = 7; pIndices[29] = 6;
    pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
    pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;

    m_pIB->Unlock();


#pragma endregion 



    return S_OK;
}


void CCollider::Set_Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
    /* 절대 비율 */
    m_vScale.x = fScaleX;
    m_vScale.y = fScaleY;
    m_vScale.z = fScaleZ;

    /* 버텍스 버퍼 다시 설정 */
    VTXCUBETEX* pVertices = { nullptr };

    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    pVertices[0].vPosition = m_vPoint[0] = _float3(-m_vScale.x, m_vScale.y, -m_vScale.z) * 0.5f + m_vPosition;
    pVertices[1].vPosition = m_vPoint[1] = _float3(m_vScale.x, m_vScale.y, -m_vScale.z) * 0.5f + m_vPosition;
    pVertices[2].vPosition = m_vPoint[2] = _float3(m_vScale.x, -m_vScale.y, -m_vScale.z) * 0.5f + m_vPosition;
    pVertices[3].vPosition = m_vPoint[3] = _float3(-m_vScale.x, -m_vScale.y, -m_vScale.z) * 0.5f + m_vPosition;
    pVertices[4].vPosition = m_vPoint[4] = _float3(-m_vScale.x, m_vScale.y, m_vScale.z) * 0.5f + m_vPosition;
    pVertices[5].vPosition = m_vPoint[5] = _float3(m_vScale.x, m_vScale.y, m_vScale.z) * 0.5f + m_vPosition;
    pVertices[6].vPosition = m_vPoint[6] = _float3(m_vScale.x, -m_vScale.y, m_vScale.z) * 0.5f + m_vPosition;
    pVertices[7].vPosition = m_vPoint[7] = _float3(-m_vScale.x, -m_vScale.y, m_vScale.z) * 0.5f + m_vPosition;

    m_pVB->Unlock();
}

_bool CCollider::GetOverlapAll(vector<class CGameObject*>*& pList)
{
    if (m_eState == COLLIDER_STATE::NONE) return false;
    pList = &m_pOthers;
    return true;
}


CGameObject* CCollider::GetOverlapTarget()
{
    /*  충돌 가장 마지막에 들어 온 객체를 꺼내오기.
        빠른 투사체 테스트는 현재 진행할 수 없어서 테스트를 못해봄.
        만약 정확하지 않으면 GetOverlapAll에서 꺼내서 이름 비교하며 처리하기*/

    return (m_eState == COLLIDER_STATE::NONE|| m_pOthers.empty()) ? nullptr : m_pOthers.back() != nullptr ? m_pOthers.back() : nullptr;
}

CCollider::TARGET_RESULT  CCollider::FindNearestTarget(_wstring strName)
{
    TARGET_RESULT result;
    
    _float3 myPos = m_pTransform->Get_State(STATE::POSITION);
    _float fMinDist = FLT_MAX;

    for (auto pOther : m_pOthers)
    {
		if (pOther == nullptr) continue;
        if (strName != TEXT("") && !pOther->CompareName(strName)) continue;

        CTransform* pOtherTransform = static_cast<CTransform*>(pOther->Get_Component(TEXT("Com_Transform")));
        if (pOtherTransform == nullptr) continue;

        _float3 otherPos = pOtherTransform->Get_State(STATE::POSITION);
		_float3 toOther = otherPos - myPos;
		_float dist = D3DXVec3LengthSq(&toOther);

		if (dist < fMinDist)
		{
			fMinDist = dist;
            result.pGameObject = pOther;
            result.fDist = D3DXVec3Length(&toOther);
            D3DXVec3Normalize(&toOther, &toOther);
            result.vDirection = toOther;
            result.vTargetPosition = otherPos;
		}
	}

    return result;
}

const COLLIDER_DIR CCollider::DetectCollisionDirection(_float* distance) const
{
    if (m_eState == COLLIDER_STATE::NONE) return COLLIDER_DIR::CD_END;
    if (m_pOthers.size() == 0) return COLLIDER_DIR::CD_END;
    _float3  myPosition = m_pTransform->Get_State(STATE::POSITION);
    _float3 myScale = m_vScale;

    CTransform* other = nullptr;
    if(!m_pOthers.empty() && m_pOthers.back() != nullptr)
        other = dynamic_cast<CTransform*>(m_pOthers.back()->Get_Component(TEXT("Com_Transform")));

    if (other == nullptr)
       return  COLLIDER_DIR::CD_END;
    
    _float3  otherPosition = other->Get_State(STATE::POSITION);
    _float3  otherScale = other->Get_Scaled();
    _float3  vDelta = otherPosition - myPosition;

    _float absX = fabsf(vDelta.x);
    _float absY = fabsf(vDelta.y);
    _float absZ = fabsf(vDelta.z);

    
    _float overlapX = (myScale.x + otherScale.x) * 0.5f - absX;
    _float overlapY = (myScale.y + otherScale.y) * 0.5f - absY;
    _float overlapZ = (myScale.z + otherScale.z) * 0.5f - absZ;

    if (overlapX <= overlapY && overlapX <= overlapZ)
    {
        if(distance != nullptr) *distance = overlapX * 0.5f;
        return (vDelta.x > 0) ? COLLIDER_DIR::LEFT : COLLIDER_DIR::RIGHT;
    }
    else if (overlapY <= overlapZ) 
    {
        if (distance != nullptr) *distance = overlapY * 0.5f;
        return (vDelta.y > 0) ? COLLIDER_DIR::BOTTOM : COLLIDER_DIR::TOP;
    }
    else 
    {
        if (distance != nullptr)*distance = overlapZ;
        return (vDelta.z > 0) ? COLLIDER_DIR::FRONT : COLLIDER_DIR::BACK;
    }
    
}

const _bool CCollider::GetCollisionsOffset(_float3* distance, const _wstring strComponentTag) const
{
    if (m_eState == COLLIDER_STATE::NONE || m_pOthers.size() == 0) return false;
    if(distance == nullptr)return false;
    _float3  result = { 0.f, 0.f, 0.f };
    _float3  myPosition = m_pTransform->Get_State(STATE::POSITION);
    _float3  myScale = m_vScale;

    _float3  vMinCompare = { FLT_MAX,FLT_MAX ,FLT_MAX };
	_bool    bFlag[3] = { false,false,false };
    _float minMagnitude = FLT_MAX;

    for (auto& iter : m_pOthers)
    {
        if (iter == nullptr) continue;

        _wstring strOtherName = iter->Get_Name();

        /* m_strCompares[]에 있는 단어가 앞에 붙어 있는 것만 검사  */
        _bool bIsPass = false;
        /* 이름에 strCompare가 붙어 있으면 검사시작*/
        for (_uint i = 0; i < m_strCompares.size(); ++i) {
            if ((strOtherName.size() >= m_strCompares[i].size() && strOtherName.substr(0, m_strCompares[i].size()).compare(m_strCompares[i]) == false))
            {
                bIsPass = true;
            }
        }
            
        if (!bIsPass) continue;

        CCollider* otherCollider = dynamic_cast<CCollider*>(iter->Get_Component(strComponentTag));

        if (otherCollider == nullptr) continue;


        _float3  otherPosition = otherCollider->Get_ColliderPosition();
        _float3  otherScale = otherCollider->Get_ColliderScaled();
   
        /* 혹시라도 콜라이더에 포지션이 없으면 */
        if (D3DXVec3Length(&otherPosition) == 0.f)
            otherPosition = dynamic_cast<CTransform*>(iter->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);

        /* 이 게임 특성상 x,z축으로만 움직이므로 y 값은 같은 값으로 고정으로 줌  */
        otherPosition.y = myPosition.y;

        _float3  vDelta = myPosition - otherPosition;
        _float absX = fabsf(vDelta.x);
        _float absY = fabsf(vDelta.y);
        _float absZ = fabsf(vDelta.z);

        _float overlapX = (myScale.x + otherScale.x) * 0.5f - absX;
        _float overlapY = (myScale.y + otherScale.y) * 0.5f - absY;
        _float overlapZ = (myScale.z + otherScale.z) * 0.5f - absZ;

        if (overlapX < 0 || overlapY < 0 || overlapZ < 0) continue;

        /* 충돌중인 모든 콜라이더에서 가장 작은 축들을 더함 */
        if (overlapX <= overlapY && overlapX <= overlapZ)
            result += { (vDelta.x > 0 ? overlapX : -overlapX), 0.f, 0.f };
        else if (overlapY <= overlapX && overlapY <= overlapZ)
            result += { 0.f, (vDelta.y > 0 ? overlapY : -overlapY), 0.f };
        else
            result += { 0.f, 0.f, (vDelta.z > 0 ? overlapZ : -overlapZ) };

        /* 가장 가까운 콜라이더 1개에서 가장 작은 축 하나 고름 */
       /* _float3 offset = { 0.f, 0.f, 0.f };

        if (overlapX <= overlapY && overlapX <= overlapZ)
            offset.x = (vDelta.x > 0) ? overlapX : -overlapX;
        else if (overlapY <= overlapX && overlapY <= overlapZ)
            offset.y = (vDelta.y > 0) ? overlapY : -overlapY;
        else
            offset.z = (vDelta.z > 0) ? overlapZ : -overlapZ;

        _float magnitude = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
        if (magnitude < minMagnitude)
        {
            minMagnitude = magnitude;
            result = offset;
        }*/
    }

    //if (minMagnitude == FLT_MAX)
    //    return false;

    *distance = result;
    return true;


}


HRESULT CCollider::Render()
{

     //세계 변환 행렬 가져오기
    _float4x4 matWorld = *m_pTransform->Get_WorldMatrix();

    // 회전 값 제거 (단위 행렬의 방향 벡터 유지)
    matWorld._11 = 1.0f; matWorld._12 = 0.0f; matWorld._13 = 0.0f;
    matWorld._21 = 0.0f; matWorld._22 = 1.0f; matWorld._23 = 0.0f;
    matWorld._31 = 0.0f; matWorld._32 = 0.0f; matWorld._33 = 1.0f;

    if (m_bIsFixed)
        memcpy(&matWorld.m[ENUM_CLASS(STATE::POSITION)][0], &m_vPosition, sizeof(_float3));


    // 변환 행렬 적용
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

    // 와이어프레임 모드 설정 및 컬모드 설정
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    // 충돌 여부에 따른 색상 적용
    if(m_eState != COLLIDER_STATE::NONE)
    {
    	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0, 0, 255, 0)); 
    	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
    }
    else
    {
    	// 원래 텍스처 사용
    	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    // 정점 및 인덱스 버퍼 설정
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
    m_pGraphic_Device->SetFVF(m_iFVF);
    m_pGraphic_Device->SetIndices(m_pIB);

    // 도형 그리기
    m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

    // 상태 복구 - 솔리드 모드로 변경 및 컬모드
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    // 상태 복구 - 텍스처 팩터 기본값으로 변경
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

    // 텍스처 단계 원래대로 복구
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


    return S_OK;
}

HRESULT CCollider::OBB_Render()
{
    //세계 변환 행렬 가져오기
    _float4x4 matWorld = *m_pTransform->Get_WorldMatrix();

    _float3 Temp = { matWorld._11 , matWorld._12, matWorld._13 };
    D3DXVec3Normalize(&Temp, &Temp);
    matWorld._11 = Temp.x; matWorld._12 = Temp.y; matWorld._13 = Temp.z;

    Temp = { matWorld._21 , matWorld._22, matWorld._23 };
    D3DXVec3Normalize(&Temp, &Temp);
    matWorld._21 = Temp.x; matWorld._22 = Temp.y; matWorld._23 = Temp.z;

    Temp = { matWorld._31 , matWorld._32, matWorld._33 };
    D3DXVec3Normalize(&Temp, &Temp);
    matWorld._31 = Temp.x; matWorld._32 = Temp.y; matWorld._33 = Temp.z;


    if (m_bIsFixed)
        memcpy(&matWorld.m[ENUM_CLASS(STATE::POSITION)][0], &m_vPosition, sizeof(_float3));


    // 변환 행렬 적용
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

    // 와이어프레임 모드 설정 및 컬모드 설정
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    // 충돌 여부에 따른 색상 적용
    if (m_eState != COLLIDER_STATE::NONE)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0, 0, 255, 0));
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
    }
    else
    {
        // 원래 텍스처 사용
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    // 정점 및 인덱스 버퍼 설정
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
    m_pGraphic_Device->SetFVF(m_iFVF);
    m_pGraphic_Device->SetIndices(m_pIB);

    // 도형 그리기
    m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

    // 상태 복구 - 솔리드 모드로 변경 및 컬모드
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    // 상태 복구 - 텍스처 팩터 기본값으로 변경
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

    // 텍스처 단계 원래대로 복구
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


    return S_OK;
}

COLLIDER_SHAPE CCollider::Reference_Collider_Info(COLLIDER_DESC& desc)
{
    desc.pOwner = m_pOwner;
    desc.pTransform = m_pTransform;
    desc.vColliderPosion = m_vPosition;
    desc.vColliderScale = m_vScale;
    desc.bIsFixed = m_bIsFixed;


    return m_eShape;
}

void CCollider::Add_Others(const vector<CGameObject*>& currentCollisions)
{
    m_pOthers = currentCollisions;

    //for (auto& pGameObject : m_pOthers)
    //    Safe_AddRef(pGameObject);

}

void CCollider::Remove_Others()
{
    //for (auto& pGameObject : m_pOthers)
    //    Safe_Release(pGameObject);
    m_pOthers.clear();
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

    Safe_Release(m_pIB);
    Safe_Release(m_pVB);

 /*   for (auto& pGameObject : m_pOthers)
        Safe_Release(pGameObject);*/
    m_pOthers.clear();

    Safe_Release(m_pTransform);

}
