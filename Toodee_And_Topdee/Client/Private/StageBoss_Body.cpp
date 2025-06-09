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

HRESULT CStageBoss_Body::Initialize_Prototype(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	Ready_PartsData();
	m_pTransformCom->Scaling(9.f, 8.f, 9.f);
	m_eState = STAGEMONERSTATE::IDLE;
	m_eViewMode = VIEWMODE::TOODEE;

	m_pTopDee = m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0);
	m_pTooDee = m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Player_TooDee"), TEXT("Com_Transform"), 0);

	_float4x4	matRotX, matRotZ, finalmat;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(130.f));
	finalmat = matRotX * matRotZ;
	m_pTransformCom->Set_Matrix(finalmat);

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();
	Ready_SubscribeEvent(m_iPlayLevel);
	m_fInitPos = m_pTransformCom->Get_State(STATE::POSITION);
	name = TEXT("WallBoss");

	return S_OK;
}

HRESULT CStageBoss_Body::Initialize(void* pArg)
{
	return S_OK;
}

void CStageBoss_Body::Priority_Update(_float fTimeDelta)
{
}

void CStageBoss_Body::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_eViewMode == VIEWMODE::TOODEE)
	{
		if (m_fIdleTime >= 2.f)
		{
			Create_Fire();
			m_fIdleTime = 0;
		}
		else
			m_fIdleTime += fTimeDelta;
	}

	Calculate_Pupil_Pos();
}

void CStageBoss_Body::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CStageBoss_Body::Render()
{
	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	Render_Parts();

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CStageBoss_Body::Render_Parts()
{
	const _float4x4* ParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	_float4x4 EyeWOrldMatrix[2] = {};
	static _bool flag = false;

	for (auto& parts : m_sParts)
	{
		_float4x4 localScale, localRotY, localRotOrbit, localTrans, local;
		D3DXMatrixScaling(&localScale, parts.fScale.x, parts.fScale.y, parts.fScale.z);
		D3DXMatrixRotationY(&localRotY, D3DXToRadian(parts.fRot));
		D3DXMatrixTranslation(&localTrans, parts.fOffset.x, parts.fOffset.y, parts.fOffset.z);

		local = localScale * localRotY * localTrans;

		_float4x4 world;
		if (parts.TextureIdx == 5)
		{
			D3DXMatrixRotationZ(&localRotOrbit, -m_fAngle);
			world = local * localRotOrbit * EyeWOrldMatrix[(flag == false) ? 0 : 1];
			flag = true;
		}
		else
			world = local * (*ParentWorldMatrix);

		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &world);

		if (FAILED(m_pPartsTextureCom->Bind_Texture(parts.TextureIdx)))
			return E_FAIL;

		parts.pVIPartsBufferCom->Bind_Buffers();
		parts.pVIPartsBufferCom->Render();

		if (parts.TextureIdx == 0)
			EyeWOrldMatrix[(parts.fOffset.x > .5f) ? 0 : 1] = world;
	}
	
	flag = false;

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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Pupil1"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil1)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Pupil2"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil2)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Corn1"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::CORN1)].pVIPartsBufferCom)))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Corn2"), reinterpret_cast<CComponent**>(&(m_sParts[ENUM_CLASS(PARTS_TYPE::CORN2)].pVIPartsBufferCom)))))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Body"),
		TEXT("Com_Texture_Body"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Parts"),
		TEXT("Com_Texture_Parts"), reinterpret_cast<CComponent**>(&m_pPartsTextureCom))))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	/*TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(40.f);*/
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = 1;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(8.f, 8.f, 8.f);		//юс╫ц
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CStageBoss_Body::Ready_PartsData()
{
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].fOffset = _float3{ .7f, .15f, .1f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].fOffset = _float3{ .1f, .15f, .7f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].fOffset = _float3{ .55f, -.2f, .5f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].fOffset = _float3{ .35f, .55f, .1f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].fOffset = _float3{ 0.f, .55f, .35f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].fOffset = _float3{ .05f, .55f, 0.f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil1)].fOffset = _float3{ .14f, .14f, .14f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil2)].fOffset = _float3{ .14f, .14f, .14f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN1)].fOffset = _float3{ .6f, .4f, .1f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN2)].fOffset = _float3{ .1f, .4f, .6f };

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].fScale = _float3{ .45f, .5f, .45f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].fScale = _float3{ .45f, .5f, .45f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].fScale = _float3{ 1.0f, .4f, .4f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].fScale = _float3{ .25f, .25f, .25f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].fScale = _float3{ .25f, .35f, .25f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].fScale = _float3{ .25f, .45f, .25f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil1)].fScale = _float3{ .6f, .6f, .6f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil2)].fScale = _float3{ .6f, .6f, .6f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN1)].fScale = _float3{ .35f, .35f, .35f };
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN2)].fScale = _float3{ .35f, .35f, .35f };

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].fRot = 25;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].fRot = 70;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].fRot = 55;
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil1)].fRot = 0;
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil2)].fRot = 0;
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN1)].fRot = 45;
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN2)].fRot = 45;

	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_L)].TextureIdx = 0;
	m_sParts[ENUM_CLASS(PARTS_TYPE::EYE_R)].TextureIdx = 0;
	m_sParts[ENUM_CLASS(PARTS_TYPE::MOUTH)].TextureIdx = 1;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN1)].TextureIdx = 2;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN2)].TextureIdx = 3;
	m_sParts[ENUM_CLASS(PARTS_TYPE::HORN3)].TextureIdx = 4;
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil1)].TextureIdx = 5;
	m_sParts[ENUM_CLASS(PARTS_TYPE::Pupil2)].TextureIdx = 5;
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN1)].TextureIdx = 6;
	m_sParts[ENUM_CLASS(PARTS_TYPE::CORN2)].TextureIdx = 7;

}

HRESULT CStageBoss_Body::Create_Fire()
{
	_float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
	//pos.z -=.f;
	pos.y = 0.f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_FireBall"),
		ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_GameObject_FireBall"), pos)))
		return E_FAIL;

	return S_OK;
}

void CStageBoss_Body::Calculate_Pupil_Pos()
{
	const _float3 fTargetPos = dynamic_cast<CTransform*>(m_pTooDee)->Get_State(STATE::POSITION);

	m_fAngle = atan2f(fTargetPos.z - m_pTransformCom->Get_State(STATE::POSITION).z,
		fTargetPos.x - m_pTransformCom->Get_State(STATE::POSITION).x) + D3DXToRadian(220);

}

CStageBoss_Body* CStageBoss_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CStageBoss_Body* pInstance = new CStageBoss_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
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
	Safe_Release(m_pColliderCom);

	for (auto& parts : m_sParts)
		Safe_Release(parts.pVIPartsBufferCom);
}
