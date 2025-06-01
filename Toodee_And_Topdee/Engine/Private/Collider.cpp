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
        m_bIsFixed = true;
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

const COLLIDER_DIR CCollider::DetectCollisionDirection(_float* distance) const
{
    if (m_eState == COLLIDER_STATE::NONE) return COLLIDER_DIR::CD_END;

    _float3  myPosition = m_pTransform->Get_State(STATE::POSITION);
    _float3  myScale = m_pTransform->Get_Scaled();

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

    if (overlapX <= overlapY && overlapX <= overlapZ) {
        *distance = overlapX * 0.5f;
        return (vDelta.x > 0) ? COLLIDER_DIR::LEFT : COLLIDER_DIR::RIGHT;
    }
    else if (overlapY <= overlapZ) {
        *distance = overlapY * 0.5f;
        return (vDelta.y > 0) ? COLLIDER_DIR::BOTTOM : COLLIDER_DIR::TOP;
    }
    else {
        *distance = overlapZ * 0.5f;
        return (vDelta.z > 0) ? COLLIDER_DIR::FRONT : COLLIDER_DIR::BACK;
    }
    

    //if (absY > absX && absY > absZ)
    //{
    //    if (vDelta.y > 0) {
    //        *distance = (otherScale.y + myScale.y) * 0.5f - vDelta.y;
    //        return COLLIDER_DIR::BOTTOM;
    //    }
    //    else
    //    {
    //        *distance = (otherScale.y + myScale.y) * 0.5f + vDelta.y;
    //        return COLLIDER_DIR::TOP;
    //    }
    //}
    //else if (absX > absZ)
    //{
    //    if (vDelta.x > 0)
    //    {
    //        *distance = (otherScale.x + myScale.x) * 0.5f - vDelta.x;
    //        return COLLIDER_DIR::LEFT;
    //    }
    //    else
    //    {
    //        *distance = (otherScale.x + myScale.x) * 0.5f + vDelta.x;
    //        return COLLIDER_DIR::RIGHT;
    //    }
    //}
    //else
    //{
    //    if (vDelta.z > 0)
    //    {            
    //        *distance = (otherScale.z + myScale.z) * 0.5f - vDelta.z;
    //        return COLLIDER_DIR::FRONT;
    //    }
    //    else
    //    {
    //        *distance = (otherScale.z + myScale.z) * 0.5f + vDelta.z;
    //        return COLLIDER_DIR::BACK;
    //    }
    //}
}

HRESULT CCollider::Render()
{

     //세계 변환 행렬 가져오기
    _float4x4 matWorld = *m_pTransform->Get_WorldMatrix();

    // 회전 값 제거 (단위 행렬의 방향 벡터 유지)
    matWorld._11 = 1.0f; matWorld._12 = 0.0f; matWorld._13 = 0.0f;
    matWorld._21 = 0.0f; matWorld._22 = 1.0f; matWorld._23 = 0.0f;
    matWorld._31 = 0.0f; matWorld._32 = 0.0f; matWorld._33 = 1.0f;

    // 변환 행렬 적용
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matWorld);

    // 와이어프레임 모드 설정
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

    // 상태 복구 - 솔리드 모드로 변경
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

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

    for (auto& pGameObject : m_pOthers)
        Safe_AddRef(pGameObject);

}

void CCollider::Remove_Others()
{
    for (auto& pGameObject : m_pOthers)
        Safe_Release(pGameObject);
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

    for (auto& pGameObject : m_pOthers)
        Safe_Release(pGameObject);
    m_pOthers.clear();

    Safe_Release(m_pTransform);

}
