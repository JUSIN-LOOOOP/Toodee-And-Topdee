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
	m_pTopDee = m_pGameInstance->Find_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0);
	
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));

	_float3* pos = static_cast<_float3*>(pArg);
	m_pTransformCom->Set_State(STATE::POSITION, *pos);
	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();
	Ready_SubscribeEvent(m_iPlayLevel);

	m_fInitPos = m_pTransformCom->Get_State(STATE::POSITION);

	name = TEXT("BossWall");


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
	__super::Update(fTimeDelta);
}

void CStageBoss_Hand::Late_Update(_float fTimeDelta)
{
	if (m_eState != STAGEMONERSTATE::DEAD)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CStageBoss_Hand::Render()
{

	Render_Shadow();

	if (m_eState == STAGEMONERSTATE::DEAD)
		return S_OK;

	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;
	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CStageBoss_Hand::Render_Shadow()
{

	_float3 vGroundPoint = _float3(0.f, -0.5f, 0.f);
	_float3 vGroundNormal = _float3(0.f, 1.f, 0.f);
	D3DXPLANE groundPlane;
	D3DXPlaneFromPointNormal(&groundPlane, &vGroundPoint, &vGroundNormal);

	_float angle = D3DXToRadian(30.f);
	_float3 lightDir = _float3(cosf(angle), 2.5f, sinf(angle));
	D3DXVec3Normalize(&lightDir, &lightDir);
	_float4 light = _float4(lightDir.x, lightDir.y, lightDir.z, 0.0f);

	_float4x4 matShadow;
	D3DXMatrixShadow(&matShadow, &light, &groundPlane);

	_float4x4 matWorld = *m_pTransformCom->Get_WorldMatrix();
	matWorld._11 = 2.f;    //matWorld._12 = matWorld._13 = 0.f;
	matWorld._22 = 2.f;     matWorld._21 = matWorld._23 = 0.f;
	matWorld._33 = 1.f;    //matWorld._31 = matWorld._32 = 0.f;
	m_matrixShadow = matShadow * matWorld;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matrixShadow);

	//텍스처 스테이지 상태 백업
	DWORD oldColorOp, oldColorArg1, oldAlphaOp, oldAlphaArg1;
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLOROP, &oldColorOp);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLORARG1, &oldColorArg1);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_ALPHAOP, &oldAlphaOp);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_ALPHAARG1, &oldAlphaArg1);

	//렌더 상태 백업
	DWORD oldAlphaEnable, oldSrcBlend, oldDestBlend, oldZWrite, oldLighting;
	m_pGraphic_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &oldAlphaEnable);
	m_pGraphic_Device->GetRenderState(D3DRS_SRCBLEND, &oldSrcBlend);
	m_pGraphic_Device->GetRenderState(D3DRS_DESTBLEND, &oldDestBlend);
	m_pGraphic_Device->GetRenderState(D3DRS_ZWRITEENABLE, &oldZWrite);
	m_pGraphic_Device->GetRenderState(D3DRS_LIGHTING, &oldLighting);

	//스텐실 버퍼
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILREF, 0x0);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);


	//그림자 렌더 설정
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(196, 8, 8, 8));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	// 그림자 렌더링
	m_VIBufferCom_Diffuse->Bind_Buffers();
	m_VIBufferCom_Diffuse->Render();

	//원복
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, oldColorArg1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, oldAlphaArg1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, oldAlphaEnable);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, oldSrcBlend);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, oldDestBlend);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, oldZWrite);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, oldLighting);
	return S_OK;
}

HRESULT CStageBoss_Hand::Ready_Components()
{
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_DiffuseCube"),
		TEXT("Com_VIBuffer2"), reinterpret_cast<CComponent**>(&m_VIBufferCom_Diffuse))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Hand"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = 1.f;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(7.f, 7.f, 7.f);
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
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
}
