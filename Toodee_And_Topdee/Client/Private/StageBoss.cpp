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

	if (FAILED(Ready_Component(TEXT("Layer_BossChain"))))
		return E_FAIL;

	m_iPlayLevel = m_pGameInstance->Get_NextLevelID();
	name = TEXT("StageBoss");
	m_eViewMode = VIEWMODE::TOODEE;
	Ready_SubscribeEvent(m_iPlayLevel);
	return S_OK;
}

void CStageBoss::Priority_Update(_float fTimeDelta)
{
	if (m_eState == STAGEMONERSTATE::DEAD)
		return ;
	///debug
	m_iDeadCount;

	if (m_eState != STAGEMONERSTATE::VIEWTURN && m_pGameInstance->Key_Down('X'))
	{
		m_eViewMode = (m_eViewMode == VIEWMODE::TOPDEE) ? VIEWMODE::TOODEE : VIEWMODE::TOPDEE;
		m_eState = STAGEMONERSTATE::VIEWTURN;
		MONSTERSIGNAL mode;
		mode.iViewMode = ENUM_CLASS(m_eViewMode);
		mode.iState = ENUM_CLASS(STAGEMONERSTATE::VIEWTURN);
 		m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::SIG_MONSTER, mode);
	}
	for (auto limb : m_vlimbs)
		limb->Priority_Update(fTimeDelta);
}

void CStageBoss::Update(_float fTimeDelta)
{
	m_fNonDamagedTime -= fTimeDelta;
	if (m_fNonDamagedTime < 0.f)
		m_fNonDamagedTime = 0;
	if (m_eState == STAGEMONERSTATE::DEAD)
		return ;

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
	if (m_eState == STAGEMONERSTATE::DEAD)
		return ;

	for (auto limb : m_vlimbs)
		limb->Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

HRESULT CStageBoss::Render()
{
	if (m_eState == STAGEMONERSTATE::DEAD)
		return S_OK;

	for (auto limb : m_vlimbs)
		limb->Render();

	_float3 BodyPos = dynamic_cast<CTransform*>(m_vlimbs[ENUM_CLASS(LIMB::BODY)]->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
	_float3 TargetPos[2] = {};
	TargetPos[0] = dynamic_cast<CTransform*>(m_vlimbs[ENUM_CLASS(LIMB::LEFT_HAND)]->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);
	TargetPos[1] = dynamic_cast<CTransform*>(m_vlimbs[ENUM_CLASS(LIMB::RIGHT_HAND)]->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);

	_float3 vMoveDir[2] = {};
	_float4x4 matRotX, matRotZ, matFinal[2];
	_float fAngle; 

	for (_uint idx = 0; idx < ENUM_CLASS(LIMB::LIMB_END) - 1; idx++)
	{
		vMoveDir[idx] = TargetPos[idx] - BodyPos;
		D3DXVec3Normalize(&vMoveDir[idx], &vMoveDir[idx]);
		fAngle = atan2f(TargetPos[idx].z - BodyPos.z, TargetPos[idx].x - BodyPos.x);
		D3DXMatrixRotationX(&matRotX, D3DXToRadian(90.f));
		D3DXMatrixRotationY(&matRotZ, -fAngle - D3DXToRadian(10.f));
		matFinal[idx] = matRotX * matRotZ;
	}

	_float4x4 WorldMatrix = {};
	_float3 vPosition = BodyPos;
	_float3 legnth;
	_uint	textureIdx = 0;
	D3DXMatrixIdentity(&WorldMatrix);

	Begin_RenderState();

	for (_uint idx = 0; idx < ENUM_CLASS(LIMB::LIMB_END) - 1; idx++)
	{
		legnth = TargetPos[idx] - vPosition;
		while (D3DXVec3Length(&legnth) > 1.f)
		{
			WorldMatrix._11 *= 2.f;
			if ((textureIdx % 2) == 0)
				WorldMatrix._22 *= 2.f;

			WorldMatrix *= matFinal[idx];
			
			vPosition += vMoveDir[idx] * 2.f;
			WorldMatrix._41 = vPosition.x;
			WorldMatrix._42 = vPosition.y;
			WorldMatrix._43 = vPosition.z;

			m_pGraphic_Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

			if (FAILED(m_pTextureCom->Bind_Texture(textureIdx % 2)))
				return E_FAIL;
			m_pVIBufferCom->Bind_Buffers();
			m_pVIBufferCom->Render();

			D3DXMatrixIdentity(&WorldMatrix);

			++textureIdx;
			legnth = TargetPos[idx] - vPosition;
		}
		vPosition = BodyPos;
	}

	End_RenderState();

	return S_OK;
}

HRESULT CStageBoss::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CStageBoss::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

void CStageBoss::isFinish()
{
	static _uint ResetFlag = 0;

	if (m_eState == STAGEMONERSTATE::DAMAGE)
	{
		++ ResetFlag;
		if (ResetFlag == 3)
		{
			m_eState = STAGEMONERSTATE::IDLE;
			ResetFlag = 0;
		}
	}
	else
		m_eState = STAGEMONERSTATE::IDLE;
}

void CStageBoss::isDamaged()
{
	if (m_fNonDamagedTime > 0.f)
		return;

	++m_iDeadCount;

	m_eViewMode = VIEWMODE::TOODEE;
	m_eState = (m_iDeadCount == 2) ? STAGEMONERSTATE::DEAD : STAGEMONERSTATE::DAMAGE;
	MONSTERSIGNAL mode;
	mode.iViewMode = ENUM_CLASS(m_eViewMode);
	mode.iState = ENUM_CLASS(m_eState);
	m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::SIG_MONSTER, mode);
	m_fNonDamagedTime = 30.f;
	FIANLBOSSRESET_EVENT tmp2;
	m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::STAGEBOSS_RESET, tmp2);

}

HRESULT CStageBoss::Ready_SubscribeEvent(_uint iPlayerLevel)
{
	m_pGameInstance->Subscribe<MONSTERSIGNAL>(iPlayerLevel, EVENT_KEY::FIN_ACTION, [this](const MONSTERSIGNAL& Event) {
		this->isFinish(); });

	m_pGameInstance->Subscribe<MONSTERDAMAGED>(iPlayerLevel, EVENT_KEY::STAGEBOSS_DAMAGED, [this](const MONSTERDAMAGED& Event) {
		this->isDamaged(); });

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

HRESULT CStageBoss::Ready_Component(const _wstring& strLayerTag)
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STAGEBOSS), TEXT("Prototype_Component_Texture_StageBoss_Chain"),
		TEXT("Com_Texture_Chain"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Chain"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

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

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
