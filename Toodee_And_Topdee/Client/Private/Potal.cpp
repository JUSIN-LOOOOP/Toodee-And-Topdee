#include "Potal.h"
#include "Collider.h"
#include "Transform.h"
#include "GameInstance.h"

CPotal::CPotal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device}
{
}

CPotal::CPotal(const CPotal& Prototype)
	: CGameObject { Prototype }
	, m_iMaxAnimCount { Prototype.m_iMaxAnimCount}
	, m_fAnimDelay { Prototype.m_fAnimDelay}
{
}

HRESULT CPotal::Initialize_Prototype()
{

	m_iMaxAnimCount = 11;
	m_fAnimDelay = 0.02f;

	return S_OK;
}

HRESULT CPotal::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

	Ready_Components();
	Ready_SubscribeEvent(m_iPlayLevel);

	m_iCurrentAnimCount = 0;
	m_fAnimTime = 0.f;

	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	_float3 vPosition = pDesc->vPos;
	vPosition.x += 1.f;
	vPosition.y += 1.5f;
	vPosition.z += 1.f;

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);

	m_pTransformCom->Scaling(4.f, 4.f, 4.f);
	m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	name = TEXT("Portal");

	m_pColliderCom->Collision_On();

	return S_OK;
}

void CPotal::Priority_Update(_float fTimeDelta)
{
}

void CPotal::Update(_float fTimeDelta)
{
	// �÷��̾� �浹 On -> Clear
}

void CPotal::Late_Update(_float fTimeDelta)
{
	if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
	{
		m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount;
		m_fAnimTime = 0.f;
	}
	else
		m_fAnimTime += fTimeDelta;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPotal::Render()
{
	m_pColliderCom->Render();

	m_pTransformCom->Bind_Matrix();

	m_pTextureCom->Bind_Texture(m_iCurrentAnimCount);

	m_pVIBufferCom->Bind_Buffers();

	Begin_RenderState();
	
	m_pVIBufferCom->Render();

	End_RenderState();


	return S_OK;
}

void CPotal::BeginOverlapPlayer(const ENTERPORTALEVENT& Event)
{
	if (nullptr == Event.pPlayer)
		return;

	pair<unordered_set<CGameObject*>::iterator, bool> ResultPair;
	ResultPair = m_OverlapSubjects.insert(Event.pPlayer);

	if (false == ResultPair.second)
		return;

	m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
	m_pGameInstance->PlayAudio(TEXT("EnterPortal.wav"), CHANNELID::SOUND_EFFECT, 0.5f);

	if (m_OverlapSubjects.size() >= 2) // 포탈에 2명 다 들어왔다면
	{
		CANCLEAREVENT Event;
		Event.vPosition = m_pTransformCom->Get_State(STATE::POSITION);

		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::CAN_CLEAR, Event);

		LOADINGSCREEN_EVENT event;
		event.bFadeIn = false;
		event.vPos = m_pTransformCom->Get_State(STATE::POSITION);
		event.vPos.y = 3.f;
		m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::LOADINGSCREEN, event);
	}
}

void CPotal::EndOverlapPlayer(const EXITPORTALEVENT& Event)
{
	if (nullptr == Event.pPlayer)
		return;

	m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
	m_pGameInstance->PlayAudio(TEXT("ExitPortal.wav"), CHANNELID::SOUND_EFFECT, 0.5f);

	_uint iErase = m_OverlapSubjects.erase(Event.pPlayer);

	if (iErase == 0)
		return;
}

HRESULT CPotal::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Potal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;

	ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
	ColliderDesc.vColliderScale = _float3(3.5f, 3.5f, 3.5f);
	ColliderDesc.bIsFixed = false;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_PortalCollider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CPotal::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<ENTERPORTALEVENT>(m_iPlayLevel, EVENT_KEY::ENTER_PORTAL, [this](const ENTERPORTALEVENT& Event) {
		this->BeginOverlapPlayer(Event);
		});

	m_pGameInstance->Subscribe<EXITPORTALEVENT>(m_iPlayLevel, EVENT_KEY::EXIT_PORTAL, [this](const EXITPORTALEVENT& Event) {
		this->EndOverlapPlayer(Event);
		});

	return S_OK;
}

HRESULT CPotal::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CPotal::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CPotal* CPotal::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPotal* pInstance = new CPotal(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed Created : CPotal"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPotal::Clone(void* pArg)
{
	CPotal* pInstance = new CPotal(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Cloned : CPotal"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPotal::Free()
{
	__super::Free();

	m_OverlapSubjects.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
