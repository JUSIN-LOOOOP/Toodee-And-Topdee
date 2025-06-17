#include "Shader_Water.h"

CShader_Water::CShader_Water(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CShader { pGraphic_Device }
{
}

CShader_Water::CShader_Water(const CShader_Water& Prototype)
	: CShader { Prototype }
{
}

HRESULT CShader_Water::Initialize_Prototype(const _tchar* pShaderFilePath)
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

HRESULT CShader_Water::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CShader_Water::Bind_Shader(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SHADERWATER_DESC* pDesc = static_cast<SHADERWATER_DESC*>(pArg);

	m_pGraphic_Device->SetVertexShader(m_pVertexShader);
	m_pGraphic_Device->SetPixelShader(m_pPixelShader);

	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_WorldMatrix", &pDesc->WorldMatrix);
	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_ViewMatrix", &pDesc->ViewMatrix);
	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_ProjMatrix", &pDesc->ProjMatrix);

	m_pVSConst->SetFloat(m_pGraphic_Device, "g_FillHeight", pDesc->fFillHeight);
	m_pVSConst->SetFloatArray(m_pGraphic_Device, "g_SinkPosition", pDesc->vSinkPosition, 3);
	m_pVSConst->SetFloat(m_pGraphic_Device, "g_SinkPower", pDesc->fSinkPower);
	m_pVSConst->SetFloat(m_pGraphic_Device, "g_TimeDelta", pDesc->fTimeDelta);
	m_pVSConst->SetFloat(m_pGraphic_Device, "g_Speed", pDesc->fSpeed);

	m_pPSConst->SetFloat(m_pGraphic_Device, "g_TimeDelta", pDesc->fTimeDelta);
	
	return S_OK;
}

CShader_Water* CShader_Water::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath)
{
	CShader_Water* pInstance = new CShader_Water(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX(TEXT("Failed Created CShader_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader_Water::Clone(void* pArg)
{
	CShader_Water* pInstance = new CShader_Water(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created CShader_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader_Water::Free()
{
	__super::Free();
}
