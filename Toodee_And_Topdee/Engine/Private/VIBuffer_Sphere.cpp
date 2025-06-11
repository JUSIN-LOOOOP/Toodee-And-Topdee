#include "VIBuffer_Sphere.h"

CVIBuffer_Sphere::CVIBuffer_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Sphere::CVIBuffer_Sphere(const CVIBuffer_Sphere& Prototype)
    : CVIBuffer {Prototype}
{
}

HRESULT CVIBuffer_Sphere::Initialize_Prototype(_uint iStackCount, _uint iSliceCount)
{

    m_iNumVertices = (iStackCount + 1) * (iSliceCount+1);
    m_iVertexStride = sizeof(VTXNORTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 ;
    m_ePrimitiveType = D3DPT_TRIANGLELIST;
    m_iNumPrimitive = (iStackCount) * (iSliceCount) * 2;
    m_iNumIndices = m_iNumPrimitive * 3;
    m_iIndexStride = 4;
    m_eIndexFormat = D3DFMT_INDEX32;

    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
        return E_FAIL;
    
    VTXNORTEX* pVertices = { nullptr };

    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
    
    for (size_t i = 0; i <= iStackCount; i++)
    {
        _float fPhi = D3DX_PI * (_float(i) / _float(iStackCount));  // Max 1 Phi
        
        _float fY = cosf(fPhi);                     // 1 -> 0 -> -1
        _float fCurrentRadius = sinf(fPhi);         // North = 0, Middle = 1, South = 0

        for (size_t j = 0; j <= iSliceCount; j++)
        {
            _float fTheta = (2.0f * D3DX_PI) * (_float(j) / _float(iSliceCount));   // Max 2 Phi

            _float fX = fCurrentRadius * cosf(fTheta);
            _float fZ = fCurrentRadius * sinf(fTheta);

            _uint iIndex = (i * (iSliceCount + 1)) + j;
            pVertices[iIndex].vPosition = _float3(fX, fY, fZ);
            pVertices[iIndex].vNormal = {};
            pVertices[iIndex].vTexcoord = _float2(_float(j) / _float(iSliceCount), _float(i) / _float(iStackCount));
        }
    }

    m_pVB->Unlock();

    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    _uint* pIndices = { nullptr };

    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    _uint			iNumIndices = { 0 };

    for (size_t i = 0; i < iStackCount; i++)
    {
        for (size_t j = 0; j < iSliceCount; j++)
        {
            _uint		iIndex = i * (iSliceCount+1) + j;

            _uint		iIndices[4] = {
                iIndex,
                iIndex + (iSliceCount+1),
                iIndex + (iSliceCount + 1) + 1,
                iIndex + 1
            };

            _float3		vSourDir, vDestDir, vNormal;

            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[2];
            pIndices[iNumIndices++] = iIndices[1];

            vSourDir = pVertices[iIndices[2]].vPosition - pVertices[iIndices[0]].vPosition;
            vDestDir = pVertices[iIndices[1]].vPosition - pVertices[iIndices[2]].vPosition;
            D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

            pVertices[iIndices[0]].vNormal += vNormal;
            pVertices[iIndices[1]].vNormal += vNormal;
            pVertices[iIndices[2]].vNormal += vNormal;

            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[3];
            pIndices[iNumIndices++] = iIndices[2];

            vSourDir = pVertices[iIndices[3]].vPosition - pVertices[iIndices[0]].vPosition;
            vDestDir = pVertices[iIndices[2]].vPosition - pVertices[iIndices[3]].vPosition;
            D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

            pVertices[iIndices[0]].vNormal += vNormal;
            pVertices[iIndices[2]].vNormal += vNormal;
            pVertices[iIndices[3]].vNormal += vNormal;
        }
    }

    for (size_t i = 0; i < m_iNumVertices; i++)
        D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Sphere::Initialize(void* pArg)
{
    return S_OK;
}

CVIBuffer_Sphere* CVIBuffer_Sphere::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iStackCount, _uint iSliceCount)
{
    CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(iStackCount, iSliceCount)))
    {
        MSG_BOX(TEXT("Failed Created : CVIBuffer_Sphere"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Sphere::Clone(void* pArg)
{
    CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed Cloned : CVIBuffer_Sphere"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVIBuffer_Sphere::Free()
{
    __super::Free();
}
