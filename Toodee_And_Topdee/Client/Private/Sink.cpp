#include "Sink.h"
#include "GameInstance.h"
#include "Water.h"

CSink::CSink(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CSink::CSink(const CSink& Prototype)
	: CGameObject { Prototype }
	, m_fTotalFillHeight {Prototype.m_fTotalFillHeight}
{
}

HRESULT CSink::Initialize_Prototype()
{
	m_fTotalFillHeight = 1.f;

	name = TEXT("Sink");

	return S_OK;
}

HRESULT CSink::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	name = TEXT("Sink");
	
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	BLOCK_INFO* pInfo = static_cast<BLOCK_INFO*>(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, pInfo->vPos);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

	if (FAILED(Ready_Water()))
		return E_FAIL;


	if (FAILED(Ready_SubscribeEvent(m_iPlayLevel)))
		return E_FAIL;

	m_fOpenHoleCount = 3.f;

	m_bOnBlock = false;

	return S_OK;
}

void CSink::Priority_Update(_float fTimeDelta)
{
	if(false == m_bOnBlock)
	{
		_float FillHeight = m_fTotalFillHeight / m_fOpenHoleCount;
		m_pWater->FillWater(FillHeight);
	}
	else
	{
		m_pWater->FillWater(0.f);
	}
}

void CSink::Update(_float fTimeDelta)
{
}

void CSink::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CSink::Render()
{
	m_pColliderCom->Render();

	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();

	m_pVIBufferCom->Render();

	End_RenderState();

	return S_OK;
}

void CSink::OnBlock()
{
	m_bOnBlock = true;
}

void CSink::OnOpen()
{
	m_bOnBlock = false;
}

void CSink::SinkBlock(const SINKBLOCKEVENT& Event)
{
	m_fOpenHoleCount--;
}

void CSink::SinkOpen(const SINKOPENEVENT& Event)
{
	m_fOpenHoleCount++;
}

HRESULT CSink::Ready_Water()
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	CWater::WATER_DESC pDesc{};
	pDesc.vPosition = vPosition;
	pDesc.ppLink = &m_pWater;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_iPlayLevel, TEXT("Layer_MapObject"),
		ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_GameObject_Water"), &pDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CSink::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGE5), TEXT("Prototype_Component_Texture_Sink"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDER_DESC ColliderDesc{};
	ColliderDesc.pOwner = this;
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.vColliderScale = _float3(2.f, 2.f, 2.f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_SinkCollider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSink::Ready_SubscribeEvent(_uint iPlayLevel)
{
	m_pGameInstance->Subscribe<SINKBLOCKEVENT>(m_iPlayLevel, EVENT_KEY::SINK_BLOCK, [this](const SINKBLOCKEVENT& Event) {
		this->SinkBlock(Event);
		});

	m_pGameInstance->Subscribe<SINKOPENEVENT>(m_iPlayLevel, EVENT_KEY::SINK_OPEN, [this](const SINKOPENEVENT& Event) {
		this->SinkOpen(Event);
		});

	return S_OK;
}

HRESULT CSink::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CSink::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CSink* CSink::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSink* pInstance = new CSink(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : Sink"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSink::Clone(void* pArg)
{
	CSink* pInstance = new CSink(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : Sink"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSink::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pWater);
}
