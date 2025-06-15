#include "Thirdee_Ending.h"
#include "GameInstance.h"

CThirdee_Ending::CThirdee_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CThirdee_Ending::CThirdee_Ending(const CThirdee_Ending& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CThirdee_Ending::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CThirdee_Ending::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 400.0f;
	m_fSizeY = 400.0f;
	m_fX = m_fWinSizeX * 0.5f - 200.f;
	m_fY = m_fWinSizeY * 0.5f;

	return S_OK;
}

void CThirdee_Ending::Priority_Update(_float fTimeDelta)
{
}

void CThirdee_Ending::Update(_float fTimeDelta)
{
	m_fTotalTime += fTimeDelta;
}

void CThirdee_Ending::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CThirdee_Ending::Render()
{
	if (FAILED(__super::Begin(m_pTransformCom)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();
	Begin_ShaderState();

	m_pVIBufferCom->Render();

	End_ShaderState();
	End_RenderState();

	if (FAILED(__super::End()))
		return E_FAIL;

	return S_OK;
}

HRESULT CThirdee_Ending::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	return S_OK;
}

HRESULT CThirdee_Ending::Begin_ShaderState()
{
	CShader_Ending::SHADERENDING_DESC ShaderDesc{};

	ShaderDesc.WorldMatrix = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ShaderDesc.ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ShaderDesc.ProjMatrix);
	ShaderDesc.TotalTime = m_fTotalTime;
	

	m_pShader->Bind_Shader(&ShaderDesc);

	return S_OK;
}

HRESULT CThirdee_Ending::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CThirdee_Ending::End_ShaderState()
{
	m_pShader->Reset_Shader();

	return S_OK;
}

HRESULT CThirdee_Ending::Ready_Components()
{
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_Component_Texture_Thirdee_Ending"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_Component_Shader_Ending"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
		return E_FAIL;

	return S_OK;
}

CThirdee_Ending* CThirdee_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CThirdee_Ending* pInstance = new CThirdee_Ending(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : CThirdee_Ending"));
	}

	return pInstance;
}

CGameObject* CThirdee_Ending::Clone(void* pArg)
{
	CThirdee_Ending* pInstance = new CThirdee_Ending(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Cloned : CThirdee_Ending"));
	}

	return pInstance;
}

void CThirdee_Ending::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShader);
}
