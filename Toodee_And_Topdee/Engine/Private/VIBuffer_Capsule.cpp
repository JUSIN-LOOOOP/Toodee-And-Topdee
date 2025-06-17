#include "VIBuffer_Capsule.h"

CVIBuffer_Capsule::CVIBuffer_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Capsule::CVIBuffer_Capsule(const CVIBuffer_Capsule& Prototype)
    : CVIBuffer { Prototype }
{
}

HRESULT CVIBuffer_Capsule::Initialize_Prototype(_uint iStackCount, _uint iSliceCount)
{
    _uint iHalfSphereStacks = iStackCount / 2;
    _uint iMiddleStacks = iStackCount;
    _float fCapsuleHeight = 1.0f;
    
    _uint iTotalStackCount = ((iHalfSphereStacks * 2) + iMiddleStacks);

    m_iNumVertices = (iTotalStackCount + 1) * (iSliceCount + 1);
    m_iVertexStride = sizeof(VTXNORTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
    m_ePrimitiveType = D3DPT_TRIANGLELIST;
    m_iNumPrimitive = (iTotalStackCount) * (iSliceCount) * 2;
    m_iNumIndices = m_iNumPrimitive * 3;
    m_iIndexStride = 4;
    m_eIndexFormat = D3DFMT_INDEX32;

    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
        return E_FAIL;

    VTXNORTEX* pVertices = { nullptr };

    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

    _uint iVertexIndex = 0;
    
    for(_uint i = 0; i< iHalfSphereStacks; i++)
    {
        _float fPhi = (D3DX_PI * 0.5f) * ((_float)(i) / (_float)(iHalfSphereStacks));
        _float fY = (fCapsuleHeight * 0.5f) + cosf(fPhi);
        _float fCurrentRadius = sinf(fPhi);

        _float fTexV = (_float)(iVertexIndex) / ((_float)iTotalStackCount * (_float)(iSliceCount + 1));

        for (_uint j = 0; j <= iSliceCount; ++j)
        {
            _float fTheta = 2.0f * D3DX_PI * (_float)(j) / (_float)iSliceCount;
            _float fX = fCurrentRadius * cosf(fTheta);
            _float fZ = fCurrentRadius * sinf(fTheta);

            pVertices[iVertexIndex].vPosition = _float3(fX, fY, fZ);
            pVertices[iVertexIndex].vNormal = pVertices[iVertexIndex].vPosition;
            D3DXVec3Normalize(&pVertices[iVertexIndex].vNormal, &pVertices[iVertexIndex].vNormal);
            pVertices[iVertexIndex++].vTexcoord = _float2((float)j / iSliceCount, fTexV);
        }
   
    }

    for (_uint i = 0; i <= iSliceCount; i++)
    {
        _float fY = (fCapsuleHeight * 0.5f) - (fCapsuleHeight * ((_float)(i) / (_float)(iSliceCount)));

        _float fTexV = (_float)(iVertexIndex) / ((_float)iTotalStackCount * (_float)(iSliceCount + 1));
       
        for (_uint j = 0; j <= iSliceCount; ++j)
        {
            _float fTheta = 2.0f * D3DX_PI * (_float)(j) / (_float)iSliceCount;
            _float fX = cosf(fTheta);
            _float fZ = sinf(fTheta);

            pVertices[iVertexIndex].vPosition = _float3(fX, fY, fZ);
            pVertices[iVertexIndex].vNormal = pVertices[iVertexIndex].vPosition;
            pVertices[iVertexIndex].vNormal.y = 0.f;
            D3DXVec3Normalize(&pVertices[iVertexIndex].vNormal, &pVertices[iVertexIndex].vNormal);
            
            pVertices[iVertexIndex++].vTexcoord = _float2((float)j / iSliceCount, fTexV);
        }
    }

    for (_uint i = 1; i <= iHalfSphereStacks; i++)
    {
        _float fPhi = (D3DX_PI * 0.5f) * (1.f - (_float)(i) / (_float)(iHalfSphereStacks));
        _float fY = ((fCapsuleHeight * 0.5f) + cosf(fPhi)) * -1.f;
        _float fCurrentRadius = sinf(fPhi);

        _float fTexV = (_float)(iVertexIndex) / ((_float)iTotalStackCount * (_float)(iSliceCount + 1));

        for (_uint j = 0; j <= iSliceCount; ++j)
        {
            _float fTheta = 2.0f * D3DX_PI * (_float)(j) / (_float)iSliceCount;
            _float fX = fCurrentRadius * cosf(fTheta);
            _float fZ = fCurrentRadius * sinf(fTheta);

            pVertices[iVertexIndex].vPosition = _float3(fX, fY, fZ);
            pVertices[iVertexIndex].vNormal = pVertices[iVertexIndex].vPosition;
            D3DXVec3Normalize(&pVertices[iVertexIndex].vNormal, &pVertices[iVertexIndex].vNormal);
            pVertices[iVertexIndex++].vTexcoord = _float2((float)j / iSliceCount, fTexV);
        }
    }


    m_pVB->Unlock();

    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    _uint* pIndices = { nullptr };

    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    _uint			iNumIndices = { 0 };

    for (size_t i = 0; i < iTotalStackCount; i++)
    {
        for (size_t j = 0; j < iSliceCount; j++)
        {
            _uint		iIndex = i * (iSliceCount + 1) + j;

            _uint		iIndices[4] = {
                iIndex,
                iIndex + (iSliceCount + 1),
                iIndex + (iSliceCount + 1) + 1,
                iIndex + 1
            };


            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[2];
            pIndices[iNumIndices++] = iIndices[1];


            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[3];
            pIndices[iNumIndices++] = iIndices[2];

        }
    }

    for (size_t i = 0; i < m_iNumVertices; i++)
        D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

    m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Capsule::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Capsule* CVIBuffer_Capsule::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iStackCount, _uint iSliceCount)
{
    CVIBuffer_Capsule* pInstance = new CVIBuffer_Capsule(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(iStackCount, iSliceCount)))
    {
        MSG_BOX(TEXT("Failed Created : CVIBuffer_Capsule"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Capsule::Clone(void* pArg)
{
    CVIBuffer_Capsule* pInstance = new CVIBuffer_Capsule(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed Cloned : CVIBuffer_Capsule"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVIBuffer_Capsule::Free()
{
    __super::Free();
}
