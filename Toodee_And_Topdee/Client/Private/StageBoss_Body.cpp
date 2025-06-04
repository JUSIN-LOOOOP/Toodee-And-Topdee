#include "StageBoss_Body.h"
#include "GameInstance.h"


CStageBoss_Body::CStageBoss_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CStageBoss_limb(pGraphic_Device)
{
}

CStageBoss_Body::CStageBoss_Body(const CStageBoss_Body& Prototype)
    : CStageBoss_limb(Prototype)
{
}

HRESULT CStageBoss_Body::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStageBoss_Body::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Scaling(8.f, 8.f, 8.f);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
	m_eState = STAGEMONERSTATE::IDLE;
	m_eViewMode = VIEWMODE::TOODEE;

	m_pTopDee = m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0);
	m_pTooDee = m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Player_TooDee"), TEXT("Com_Transform"), 0);

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].fOffset = _float3{ .7f, .3f, .1f  };
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].fOffset = _float3{ .1f, .3f, .7f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].fOffset = _float3{ .55f, .0f, .5f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].fOffset = _float3{ .35f, .55f, .1f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].fOffset = _float3{ 0.f, .55f, .35f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].fOffset = _float3{ .05f, .55f, 0.f };

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].fScale = _float3{ .4f, .4f, .4f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].fScale = _float3{ .4f, .4f, .4f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].fScale = _float3{ 1.2f, .5f, .5f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].fScale = _float3{ .2f, .2f, .2f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].fScale = _float3{ .2f, .3f, .2f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].fScale = _float3{ .2f, .4f, .2f };

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].fRot = 25;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].fRot = 70;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].fRot = 55;

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].TextureIdx = 0;
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].TextureIdx = 0;
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].TextureIdx = 1;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].TextureIdx = 2;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].TextureIdx = 3;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].TextureIdx = 4;

    name = TEXT("StageBoss_Body");

    return S_OK;
}

void CStageBoss_Body::Priority_Update(_float fTimeDelta)
{
	//시그널 확인 (ViewMode, STAGEMONERSTATE)
	/*
	ViewMode는 그냥 넣어주고
	STAGEMONERSTATE는 ViewMode에 따라서 상태 다르게 넣어주기 -> 이 함수만 Hand랑 다르게 처리하면 될듯
	*/
	
	//임시 시그널 
	if (m_pGameInstance->Key_Down('X'))
	{
		if (m_eState == STAGEMONERSTATE::VIEWTURN)
			return;
		m_eViewMode = (m_eViewMode == VIEWMODE::TOPDEE) ? VIEWMODE::TOODEE : VIEWMODE::TOPDEE;
		m_eState = STAGEMONERSTATE::VIEWTURN;
	}
}

void CStageBoss_Body::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	switch (m_eState)
	{
	case STAGEMONERSTATE::CHASE:
		Chase(fTimeDelta);
		break;
	case STAGEMONERSTATE::TURN:
		Turn(fTimeDelta);
		break;
	case STAGEMONERSTATE::ATTACK:
		HIT(fTimeDelta);
		break;
	case STAGEMONERSTATE::VIEWTURN:
		ChangeView(fTimeDelta);
		break;
	case STAGEMONERSTATE::FIRE:
		Create_Fire();
		break;
	}

	//회전하는 건 플래그 다르게 줘서 분기처리

}

void CStageBoss_Body::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CStageBoss_Body::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& parts : m_sParts)	
	{
		const _float4x4* ParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();

		D3DXMATRIX localScale, localRot, localTrans, local;
		D3DXMatrixScaling(&localScale, parts.fScale.x, parts.fScale.y, parts.fScale.z);
		D3DXMatrixRotationY(&localRot, D3DXToRadian(parts.fRot));
		D3DXMatrixTranslation(&localTrans, parts.fOffset.x, parts.fOffset.y, parts.fOffset.z);

		local = localScale * localRot * localTrans;
		_float4x4 world = local * (*ParentWorldMatrix);

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &world);

		if (FAILED(m_pPartsTextureCom->Bind_Texture(parts.TextureIdx)))
			return E_FAIL;

		parts.pVIPartsBufferCom->Bind_Buffers();
		parts.pVIPartsBufferCom->Render();
	}

    return S_OK;
}

HRESULT CStageBoss_Body::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_LeftEye"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_RightEye"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Mouth"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Horn1"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Horn2"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Horn3"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].pVIPartsBufferCom)))))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_StageBoss_Body"),
		TEXT("Com_Texture_Body"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_StageBoss_Parts"),
		TEXT("Com_Texture_Parts"), reinterpret_cast<CComponent**>(&m_pPartsTextureCom))))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 15.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(40.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Body"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageBoss_Body::Create_Fire()
{
	//플레그 바꿔주는 거 까먹지 말기
	return S_OK;
}

CStageBoss_Body* CStageBoss_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStageBoss_Body* pInstance = new CStageBoss_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CStageBoss_Body"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageBoss_Body::Clone(void* pArg)
{
	CStageBoss_Body* pInstance = new CStageBoss_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CStageBoss_Body"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageBoss_Body::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pPartsTextureCom);

	for (auto& parts : m_sParts)
		Safe_Release(parts.pVIPartsBufferCom);
}
