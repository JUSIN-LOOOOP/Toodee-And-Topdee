#include "Shader.h"

CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent {pGraphic_Device }
{
}

CShader::CShader(const CShader& Prototype)
    : CComponent { Prototype }
    , m_pVertexShader { Prototype.m_pVertexShader}
    , m_pPixelShader { Prototype.m_pPixelShader }
    , m_pVSConst { Prototype.m_pVSConst}
    , m_pPSConst { Prototype.m_pPSConst }
{
    Safe_AddRef(m_pVertexShader);
    Safe_AddRef(m_pPixelShader);
    Safe_AddRef(m_pVSConst);
    Safe_AddRef(m_pPSConst);

}

HRESULT CShader::Bind_Shader(void* pArg)
{
    m_pGraphic_Device->SetVertexShader(m_pVertexShader);
    m_pGraphic_Device->SetPixelShader(m_pPixelShader);

    return S_OK;
}

HRESULT CShader::Reset_Shader()
{
    m_pGraphic_Device->SetVertexShader(nullptr);
    m_pGraphic_Device->SetPixelShader(nullptr);

    return S_OK;
}

void CShader::Free()
{
    __super::Free();

    Safe_Release(m_pVertexShader);
    Safe_Release(m_pPixelShader);
    Safe_Release(m_pVSConst);
    Safe_Release(m_pPSConst);
}
