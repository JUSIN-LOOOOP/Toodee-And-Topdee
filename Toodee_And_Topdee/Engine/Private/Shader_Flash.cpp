#include "Shader_Flash.h"

CShader_Flash::CShader_Flash(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CShader{ pGraphic_Device }
{
}

CShader_Flash::CShader_Flash(const CShader_Flash& Prototype)
	: CShader{ Prototype }
{
}

HRESULT CShader_Flash::Initialize_Prototype(const _tchar* pShaderFilePath)
{
	LPD3DXBUFFER pVSCode = nullptr;
	LPD3DXBUFFER pPSCode = nullptr;
	LPD3DXBUFFER pError = nullptr;
	HRESULT hr;

	hr = D3DXCompileShaderFromFile(pShaderFilePath, nullptr, nullptr, "VS_Main", "vs_2_0", 0, &pVSCode, &pError, &m_pVSConst);

	if (FAILED(hr))
	{
		if (pError) {
			MessageBoxA(nullptr, (char*)pError->GetBufferPointer(), "Shader Compile Error", MB_OK);
		}
		return E_FAIL;
	}

	m_pGraphic_Device->CreateVertexShader((DWORD*)pVSCode->GetBufferPointer(), &m_pVertexShader);

	hr = D3DXCompileShaderFromFile(pShaderFilePath, nullptr, nullptr, "PS_Main", "ps_2_0", 0, &pPSCode, &pError, &m_pPSConst);

	if (FAILED(hr))
	{
		if (pError) {
			MessageBoxA(nullptr, (char*)pError->GetBufferPointer(), "Shader Compile Error", MB_OK);
		}
		return E_FAIL;
	}

	m_pGraphic_Device->CreatePixelShader((DWORD*)pPSCode->GetBufferPointer(), &m_pPixelShader);

	Safe_Release(pVSCode);
	Safe_Release(pPSCode);

	return S_OK;
}

HRESULT CShader_Flash::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader_Flash::Bind_Shader(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SHADERFLASH_DESC* pDesc = static_cast<SHADERFLASH_DESC*>(pArg);

	m_pGraphic_Device->SetVertexShader(m_pVertexShader);
	m_pGraphic_Device->SetPixelShader(m_pPixelShader);

	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_WorldMatrix", &pDesc->WorldMatrix);
	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_ViewMatrix", &pDesc->ViewMatrix);
	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_ProjMatrix", &pDesc->ProjMatrix);


	m_pPSConst->SetFloat(m_pGraphic_Device, "g_TotalTime", pDesc->TotalTime);

	return S_OK;
}

CShader_Flash* CShader_Flash::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath)
{
	CShader_Flash* pInstance = new CShader_Flash(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX(TEXT("Failed Created CShader_Flash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader_Flash::Clone(void* pArg)
{
	CShader_Flash* pInstance = new CShader_Flash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created CShader_Flash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader_Flash::Free()
{
	__super::Free();
}