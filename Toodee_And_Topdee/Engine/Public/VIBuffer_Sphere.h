#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
private:
	CVIBuffer_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Sphere(const CVIBuffer_Sphere& Prototype);
	virtual ~CVIBuffer_Sphere() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iStackCount, _uint iSliceCount);
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CVIBuffer_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iStackCount, _uint iSliceCount);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END