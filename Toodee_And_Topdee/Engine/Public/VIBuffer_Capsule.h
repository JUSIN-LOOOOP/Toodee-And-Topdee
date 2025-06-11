#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Capsule final : public CVIBuffer
{
private:
	CVIBuffer_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Capsule(const CVIBuffer_Capsule& Prototype);
	virtual ~CVIBuffer_Capsule() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iStackCount, _uint iSliceCount);
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CVIBuffer_Capsule* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iStackCount, _uint iSliceCount);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END