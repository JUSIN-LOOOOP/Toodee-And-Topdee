#include "Shader_Player.h"

CShader_Player::CShader_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CShader{ pGraphic_Device }
{
}

CShader_Player::CShader_Player(const CShader_Player& Prototype)
	: CShader{ Prototype }
{
}

HRESULT CShader_Player::Initialize_Prototype(const _tchar* pShaderFilePath)
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

HRESULT CShader_Player::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CShader_Player::Bind_Shader(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SHADERPLAYER_DESC* pDesc = static_cast<SHADERPLAYER_DESC*>(pArg);

	m_pGraphic_Device->SetVertexShader(m_pVertexShader);
	m_pGraphic_Device->SetPixelShader(m_pPixelShader);

	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_WorldMatrix", &pDesc->WorldMatrix);
	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_ViewMatrix", &pDesc->ViewMatrix);
	m_pVSConst->SetMatrix(m_pGraphic_Device, "g_ProjMatrix", &pDesc->ProjMatrix);

	return S_OK;
}

CShader_Player* CShader_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath)
{
	CShader_Player* pInstance = new CShader_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX(TEXT("Failed Created CShader_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader_Player::Clone(void* pArg)
{
	CShader_Player* pInstance = new CShader_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created CShader_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader_Player::Free()
{
	__super::Free();
}
