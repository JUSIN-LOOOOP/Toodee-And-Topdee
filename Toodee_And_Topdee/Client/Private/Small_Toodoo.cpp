#include "Small_Toodoo.h"
#include "GameInstance.h"

CSmall_Toodoo::CSmall_Toodoo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CSmall_Toodoo::CSmall_Toodoo(const CSmall_Toodoo& Prototype)
	: CGameObject { Prototype }
{
}

HRESULT CSmall_Toodoo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSmall_Toodoo::Initialize(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	_float3 vPosition = _float3(17.f, 1.f, -1.f);

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
	m_pTransformCom->Scaling(10.f, 20.f, 4.f);

	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_fTotalTime = 0.f;

	return S_OK;
}

void CSmall_Toodoo::Priority_Update(_float fTimeDelta)
{
}

void CSmall_Toodoo::Update(_float fTimeDelta)
{
	m_fTotalTime += fTimeDelta;
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float fY = (-1.f) + ((sin(m_fTotalTime) * 2.f));
	vPosition.z = fY;

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
	

}

void CSmall_Toodoo::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CSmall_Toodoo::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}

HRESULT CSmall_Toodoo::Ready_Component()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_DIALOGUE), TEXT("Prototype_Component_Texture_SmallToodoo"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CSmall_Toodoo::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSmall_Toodoo::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CSmall_Toodoo* CSmall_Toodoo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSmall_Toodoo* pInstance = new CSmall_Toodoo(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CSmall_Toodoo"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSmall_Toodoo::Clone(void* pArg)
{
	CSmall_Toodoo* pInstance = new CSmall_Toodoo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CSmall_Toodoo"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSmall_Toodoo::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
