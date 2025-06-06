#include "StageBoss_Hand.h"
#include "GameInstance.h"


CStageBoss_Hand::CStageBoss_Hand(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CStageBoss_limb(pGraphic_Device)
{
}

CStageBoss_Hand::CStageBoss_Hand(const CStageBoss_Hand& Prototype)
    : CStageBoss_limb(Prototype)
{
}

HRESULT CStageBoss_Hand::Initialize_Prototype(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Scaling(6.f, 6.f, 6.f);
	m_eState = STAGEMONERSTATE::IDLE;
	m_eViewMode = VIEWMODE::TOODEE;

	m_pTopDee = m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0);

	//m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));

	_float4x4	matRotX, matRotZ, finalmat;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
	finalmat = matRotX * matRotZ;
	m_pTransformCom->Set_Matrix(finalmat);

	_float3* pos = static_cast<_float3*>(pArg);
	m_pTransformCom->Set_State(STATE::POSITION, *pos);
	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();
	Ready_SubscribeEvent(m_iPlayLevel);

	name = TEXT("StageBoss_Hand");


    return S_OK;
}

HRESULT CStageBoss_Hand::Initialize(void* pArg)
{

    return S_OK;
}

void CStageBoss_Hand::Priority_Update(_float fTimeDelta)
{
}

void CStageBoss_Hand::Update(_float fTimeDelta)
{
	//일단 Topdee모드일 때
	__super::Update(fTimeDelta);
}

void CStageBoss_Hand::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CStageBoss_Hand::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CStageBoss_Hand::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_StageBoss_Hand"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(40.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CStageBoss_Hand* CStageBoss_Hand::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CStageBoss_Hand* pInstance = new CStageBoss_Hand(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CStageBoss_Hand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageBoss_Hand::Clone(void* pArg)
{
	CStageBoss_Hand* pInstance = new CStageBoss_Hand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CStageBoss_Hand"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageBoss_Hand::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);


}
