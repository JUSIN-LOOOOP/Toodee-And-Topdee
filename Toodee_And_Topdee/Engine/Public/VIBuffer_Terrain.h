#pragma once

#include "VIBuffer.h"

/* terrain 그리기 */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ); /* 평면용 */
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);	/* height map 용*/

	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint				m_iNumVerticesX = {};
	_uint				m_iNumVerticesZ = {};

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END