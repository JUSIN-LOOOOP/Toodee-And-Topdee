#pragma once
#include "Component.h"

/* Vertex + Index  버퍼의 메모리 공간이 되는 기본 클래스 */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Buffers();
	virtual HRESULT Render();

protected:
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9				m_pIB = { nullptr };

	_uint								m_iNumVertices = { };	/* 정점의 갯수 */
	_uint								m_iVertexStride = { }; /* 정점하나의 크기(Byte) */
	_uint								m_iFVF = {};			/* 정점의 좌표 정보를 어떤걸 들고 있을 것인지 */
	D3DPRIMITIVETYPE					m_ePrimitiveType = {};	/* 어떤 방식으로 그릴 것인지 */
	_uint								m_iNumPrimitive = {};	/* 기본형의 갯수  */

	_uint								m_iNumIndices = {};		/* 인덱스의 갯수 */
	_uint								m_iIndexStride = { };	/* 인덱스 하나의 크기 */
	D3DFORMAT							m_eIndexFormat = {};	/* 인덱스 버퍼 어떤걸 사용할 것 인지 */

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END