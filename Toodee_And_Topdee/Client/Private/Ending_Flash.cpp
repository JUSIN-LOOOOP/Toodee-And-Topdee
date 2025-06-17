#include "Ending_Flash.h"
#include "GameInstance.h"

CEnding_Flash::CEnding_Flash(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CEnding_Flash::CEnding_Flash(const CEnding_Flash& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CEnding_Flash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnding_Flash::Initialize(void* pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fSizeX = 2000.f;
	m_fSizeY = 2000.f;
	m_fX = m_fWinSizeX * 0.5f - 200.f;
	m_fY = m_fWinSizeY * 0.5f;

	return S_OK;
}

void CEnding_Flash::Priority_Update(_float fTimeDelta)
{
}

void CEnding_Flash::Update(_float fTimeDelta)
{
	m_fTotalTime += fTimeDelta;
	
	if (m_fTotalTime >= 2.5f)
	{
		if(!m_bRender)
		{
			m_bRender = true;
			m_fTotalTime = 0.f;
		}
	}

}

void CEnding_Flash::Late_Update(_float fTimeDelta)
{
	if(m_bRender)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CEnding_Flash::Render()
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

HRESULT CEnding_Flash::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 20);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	return S_OK;
}

HRESULT CEnding_Flash::Begin_ShaderState()
{
	CShader_Ending::SHADERENDING_DESC ShaderDesc{};

	ShaderDesc.WorldMatrix = *m_pTransformCom->Get_WorldMatrix();
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ShaderDesc.ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ShaderDesc.ProjMatrix);
	ShaderDesc.TotalTime = m_fTotalTime;


	m_pShader->Bind_Shader(&ShaderDesc);

	return S_OK;
}

HRESULT CEnding_Flash::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CEnding_Flash::End_ShaderState()
{
	m_pShader->Reset_Shader();

	return S_OK;
}

HRESULT CEnding_Flash::Ready_Components()
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

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_Component_Texture_Flash"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_ENDING), TEXT("Prototype_Component_Shader_Flash"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
		return E_FAIL;

	return S_OK;
}

CEnding_Flash* CEnding_Flash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnding_Flash* pInstance = new CEnding_Flash(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Created : CEnding_Flash"));
	}

	return pInstance;
}

CGameObject* CEnding_Flash::Clone(void* pArg)
{
	CEnding_Flash* pInstance = new CEnding_Flash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX(TEXT("Failed Cloned : CEnding_Flash"));
	}

	return pInstance;
}

void CEnding_Flash::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShader);
}
