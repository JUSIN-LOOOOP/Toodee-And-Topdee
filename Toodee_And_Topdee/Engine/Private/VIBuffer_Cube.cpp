#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{
	m_iNumVertices = 24;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_iNumIndices = 36;
	m_iIndexStride = 2;
	m_eIndexFormat = D3DFMT_INDEX16;


#pragma region VERTEX_BUFFER
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;


	_float3 fVertexCube[8] =
	{
		{-0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},

		{-0.5f, -0.5f, 0.5f}, //7
		{0.5f, -0.5f, 0.5f},  //6
		{0.5f, 0.5f, 0.5f},  //5
		{-0.5f, 0.5f, 0.5f} //4
	};

	VTXPOSTEX* pVertices = { nullptr };

	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	//Front 
	pVertices[0].vPosition = fVertexCube[0];
	pVertices[0].vTexcoord = {0.f, 0.f};
	pVertices[1].vPosition = fVertexCube[1];
	pVertices[1].vTexcoord = { 0.f, 1.f };
	pVertices[2].vPosition = fVertexCube[2];
	pVertices[2].vTexcoord = { 1.f, 1.f };
	pVertices[3].vPosition = fVertexCube[3];
	pVertices[3].vTexcoord = { 1.f,0.f };

	//Back 
	pVertices[4].vPosition = fVertexCube[7];
	pVertices[4].vTexcoord = { 0.f, 0.f };
	pVertices[5].vPosition = fVertexCube[6];
	pVertices[5].vTexcoord = { 0.f, 1.f };
	pVertices[6].vPosition = fVertexCube[5];
	pVertices[6].vTexcoord = { 1.f, 1.f };
	pVertices[7].vPosition = fVertexCube[4];
	pVertices[7].vTexcoord = { 1.f,0.f };

	//Right
	pVertices[8].vPosition = fVertexCube[1];
	pVertices[8].vTexcoord = { 0.f, 0.f };
	pVertices[9].vPosition = fVertexCube[5];
	pVertices[9].vTexcoord = { 0.f, 1.f };
	pVertices[10].vPosition = fVertexCube[6];
	pVertices[10].vTexcoord = { 1.f, 1.f };
	pVertices[11].vPosition = fVertexCube[2];
	pVertices[11].vTexcoord = { 1.f,0.f };

	//Left
	pVertices[12].vPosition = fVertexCube[7];
	pVertices[12].vTexcoord = { 0.f, 0.f };
	pVertices[13].vPosition = fVertexCube[4];
	pVertices[13].vTexcoord = { 0.f, 1.f };
	pVertices[14].vPosition = fVertexCube[0];
	pVertices[14].vTexcoord = { 1.f, 1.f };
	pVertices[15].vPosition = fVertexCube[3];
	pVertices[15].vTexcoord = { 1.f,0.f };

	//Up
	pVertices[16].vPosition = fVertexCube[4];
	pVertices[16].vTexcoord = { 0.f, 0.f };
	pVertices[17].vPosition = fVertexCube[5];
	pVertices[17].vTexcoord = { 0.f, 1.f };
	pVertices[18].vPosition = fVertexCube[1];
	pVertices[18].vTexcoord = { 1.f, 1.f };
	pVertices[19].vPosition = fVertexCube[0];
	pVertices[19].vTexcoord = { 1.f,0.f };

	//Down
	pVertices[20].vPosition = fVertexCube[3];
	pVertices[20].vTexcoord = { 0.f, 0.f };
	pVertices[21].vPosition = fVertexCube[2];
	pVertices[21].vTexcoord = { 0.f, 1.f };
	pVertices[22].vPosition = fVertexCube[6];
	pVertices[22].vTexcoord = { 1.f, 1.f };
	pVertices[23].vPosition = fVertexCube[7];
	pVertices[23].vTexcoord = { 1.f,0.f };

	m_pVB->Unlock();

#pragma endregion 


#pragma region INDEX_BUFFER

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	_uint	iNumCnt = { 0 };
	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);


	//Front 
	pIndices[iNumCnt++] = 0;
	pIndices[iNumCnt++] = 1;
	pIndices[iNumCnt++] = 2;

	pIndices[iNumCnt++] = 0;
	pIndices[iNumCnt++] = 2;
	pIndices[iNumCnt++] = 3;

	//Back 
	pIndices[iNumCnt++] = 7;
	pIndices[iNumCnt++] = 6;
	pIndices[iNumCnt++] = 5;

	pIndices[iNumCnt++] = 7;
	pIndices[iNumCnt++] = 5;
	pIndices[iNumCnt++] = 4;

	//Right
	pIndices[iNumCnt++] = 1;
	pIndices[iNumCnt++] = 5;
	pIndices[iNumCnt++] = 6;

	pIndices[iNumCnt++] = 1;
	pIndices[iNumCnt++] = 6;
	pIndices[iNumCnt++] = 2;

	//Left
	pIndices[iNumCnt++] = 7;
	pIndices[iNumCnt++] = 4;
	pIndices[iNumCnt++] = 0;

	pIndices[iNumCnt++] = 7;
	pIndices[iNumCnt++] = 0;
	pIndices[iNumCnt++] = 3;

	//Up
	pIndices[iNumCnt++] = 4;
	pIndices[iNumCnt++] = 5;
	pIndices[iNumCnt++] = 1;

	pIndices[iNumCnt++] = 4;
	pIndices[iNumCnt++] = 1;
	pIndices[iNumCnt++] = 0;

	//Down
	pIndices[iNumCnt++] = 3;
	pIndices[iNumCnt++] = 2;
	pIndices[iNumCnt++] = 6;

	pIndices[iNumCnt++] = 3;
	pIndices[iNumCnt++] = 6;
	pIndices[iNumCnt++] = 7;

	m_pIB->Unlock();


#pragma endregion 


	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Cube"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}

