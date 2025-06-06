#include "StageBoss.h"
#include "GameInstance.h"
#include "StageBoss_Body.h"
#include "StageBoss_Hand.h"

CStageBoss::CStageBoss(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CStageBoss::CStageBoss(const CStageBoss& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CStageBoss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStageBoss::Initialize(void* pArg)
{
	if (FAILED(Ready_LimbObject(TEXT("Layer_BossLimb"))))
		return E_FAIL;

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();
	name = TEXT("StageBoss");
	m_eViewMode = VIEWMODE::TOODEE;
	Ready_SubscribeEvent(m_iPlayLevel);
	return S_OK;
}

void CStageBoss::Priority_Update(_float fTimeDelta)
{
	if (m_eState != STAGEMONERSTATE::VIEWTURN && m_pGameInstance->Key_Down('X'))
	{
		m_eViewMode = (m_eViewMode == VIEWMODE::TOPDEE) ? VIEWMODE::TOODEE : VIEWMODE::TOPDEE;
		m_eState = STAGEMONERSTATE::VIEWTURN;
		MONSTERSIGNAL mode;
		mode.iViewMode = ENUM_CLASS(m_eViewMode);
		mode.iState = ENUM_CLASS(STAGEMONERSTATE::VIEWTURN);
 		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::CHANGE_VIEW, mode);
	}
	for (auto limb : m_vlimbs)
		limb->Priority_Update(fTimeDelta);
}

void CStageBoss::Update(_float fTimeDelta)
{
	for (auto limb : m_vlimbs)
		limb->Update(fTimeDelta);

	if (m_eViewMode == VIEWMODE::TOODEE)
		return;

	if (m_eState == STAGEMONERSTATE::IDLE)
	{
		m_fIdleTime += fTimeDelta;
		if (m_fIdleTime >= 1.5f)
		{
			m_eState = STAGEMONERSTATE::ATTACK;
			m_vlimbs[rand() % ENUM_CLASS(LIMB::LIMB_END)]->Do_Attack();
			m_fIdleTime = 0.f;
		}
	}
}

void CStageBoss::Late_Update(_float fTimeDelta)
{
	for (auto limb : m_vlimbs)
		limb->Late_Update(fTimeDelta);

}

HRESULT CStageBoss::Render()
{
	for (auto limb : m_vlimbs)
		limb->Render();

	return S_OK;
}

void CStageBoss::isFinish()
{
	m_eState = STAGEMONERSTATE::IDLE;
}

HRESULT CStageBoss::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<MONSTERSIGNAL>(iPlayerLevel, EVENT_KEY::FIN_ACTION, [this](const MONSTERSIGNAL& Event) {
		this->isFinish(); });
	return S_OK;
}

HRESULT CStageBoss::Ready_LimbObject(const _wstring& strLayerTag)
{
	_float3 pos = { -12.f , 5.f, 0.f };
	CStageBoss_limb* limb = CStageBoss_Body::Create(m_pGraphic_Device, pos);
	m_vlimbs.push_back(limb);

	pos = { - 12.f , 5.f, 0.f };
	limb = CStageBoss_Hand::Create(m_pGraphic_Device, pos);
	m_vlimbs.push_back(limb);

	pos = { 12.f , 5.f, 0.f };
	limb = CStageBoss_Hand::Create(m_pGraphic_Device, pos);
	m_vlimbs.push_back(limb);

	return S_OK;
}



CStageBoss* CStageBoss::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStageBoss* pInstance = new CStageBoss(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CStageBoss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStageBoss::Clone(void* pArg)
{
	CStageBoss* pInstance = new CStageBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CStageBoss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageBoss::Free()
{
	__super::Free();

	for (auto limb : m_vlimbs)
		Safe_Release(limb);

}
