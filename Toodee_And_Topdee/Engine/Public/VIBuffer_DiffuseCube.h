#pragma once
#include "VIBuffer.h" 

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_DiffuseCube final : public CVIBuffer
{
public:
    explicit CVIBuffer_DiffuseCube(LPDIRECT3DDEVICE9 pGraphic_Device);
    CVIBuffer_DiffuseCube(const CVIBuffer_DiffuseCube& rhs);
    virtual ~CVIBuffer_DiffuseCube() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    static CVIBuffer_DiffuseCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;

    LPDIRECT3DVERTEXBUFFER9 GetVB() { return m_pVB; };
    LPDIRECT3DINDEXBUFFER9 GetIB() { return m_pIB; };
};

END