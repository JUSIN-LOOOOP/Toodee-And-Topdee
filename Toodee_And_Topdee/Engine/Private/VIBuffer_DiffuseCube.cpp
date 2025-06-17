
#include "VIBuffer_DiffuseCube.h"

CVIBuffer_DiffuseCube::CVIBuffer_DiffuseCube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_DiffuseCube::CVIBuffer_DiffuseCube(const CVIBuffer_DiffuseCube& rhs)
    : CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_DiffuseCube::Initialize_Prototype()
{
    m_iNumVertices = 8;
    m_iVertexStride = sizeof(VTXCUBEDIFFUSE);
    m_iFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    m_ePrimitiveType = D3DPT_TRIANGLELIST;
    m_iNumPrimitive = 12;
    m_iIndexStride = 2;
    m_iNumIndices = 36;
    m_eIndexFormat = D3DFMT_INDEX16;

    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
        return E_FAIL;

    VTXCUBEDIFFUSE* pVertices = nullptr;
    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    pVertices[0].vPosition = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
    pVertices[1].vPosition = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
    pVertices[2].vPosition = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
    pVertices[3].vPosition = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
    pVertices[4].vPosition = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
    pVertices[5].vPosition = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
    pVertices[6].vPosition = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
    pVertices[7].vPosition = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

    for (int i = 0; i < 8; ++i)
        pVertices[i].dwColor = D3DCOLOR_ARGB(128, 0, 0, 0);

    m_pVB->Unlock();

    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    _ushort* pIndices = nullptr;
    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    _ushort indices[36] = {
        1, 5, 6, 1, 6, 2,  // +x
        4, 0, 3, 4, 3, 7,  // -x
        4, 5, 1, 4, 1, 0,  // +y
        3, 2, 6, 3, 6, 7,  // -y
        5, 4, 7, 5, 7, 6,  // +z
        0, 1, 2, 0, 2, 3   // -z
    };
    memcpy(pIndices, indices, sizeof(indices));

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_DiffuseCube::Initialize(void* pArg)
{
    return S_OK;
}

CVIBuffer_DiffuseCube* CVIBuffer_DiffuseCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CVIBuffer_DiffuseCube* pInstance = new CVIBuffer_DiffuseCube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVIBuffer_DiffuseCube"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CVIBuffer_DiffuseCube::Clone(void* pArg)
{
    CVIBuffer_DiffuseCube* pInstance = new CVIBuffer_DiffuseCube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_DiffuseCube"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CVIBuffer_DiffuseCube::Free()
{
    __super::Free();
}
