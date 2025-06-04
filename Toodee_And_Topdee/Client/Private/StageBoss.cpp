#include "StageBoss.h"
#include "GameInstance.h"

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
	//몸통, 오른팔, 왼팔 생성 후에 Vector에 넣어주기
	//생성할 때 this 포인터 넣어주기
	//	->이걸로 attack 끝나고 시그널 주기

	name = TEXT("StageBoss");

	return S_OK;
}

void CStageBoss::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down('X'))
	{
		m_eViewMode = (m_eViewMode == VIEWMODE::TOPDEE) ? VIEWMODE::TOODEE : VIEWMODE::TOPDEE;
		//시그널 보내기
	}
}

void CStageBoss::Update(_float fTimeDelta)
{
	//if (m_eState == STAGEMONERSTATE::IDLE)
	//{
	//	m_fIdleTime += fTimeDelta;
	//	if (m_fIdleTime >= 3.f)
	//	{
	//		/*_uint idx = rand() % LIMB::LIMB_END;
	//		*m_eState = STATE::ATTACK;*/
	//	}
	//}
	//else if (m_eState == STAGEMONERSTATE::ATTACK)
	//{
	//	/*
	//	* limbs[idx]->attack();
	//	*/
	//}

	/*
	if(2D모드)
		limbs[BODY]->2Dattack();
	
	else if (3D모드)
		limbs[idx]->3Dattack();

		2D -> Head 공격 && 다른 limb (hand)는 멈춤
		3D -> 랜덤공격

		보내야할 이벤트
		1. 시점이 변화함		-> 회전 + HEAD에서는 플래그도 설정 (attack 시그널 받으면 어떤 attack을 할 지 Update안에서 분기문 처리하려고)
		2. 공격 명령

		받아야 할 이벤트
		3. 공격이 끝남

		얘가 가져야하는 STATE는 ATTACK , IDLE 
	*/

	if (m_eState == STAGEMONERSTATE::IDLE)
	{
		m_fIdleTime += fTimeDelta;
		if (m_fIdleTime >= 3.f)
		{
			m_eState = STAGEMONERSTATE::ATTACK;
			m_fIdleTime = 0.f;
		}
	}
	
	if (m_eState == STAGEMONERSTATE::ATTACK)
	{
		if (m_eViewMode == VIEWMODE::TOODEE)
			//limbs[BODY]->attack();	//어텍시그널
			;
		else
		{
			_uint idx = rand() % ENUM_CLASS(LIMB::LIMB_END);
			//limbs[idx]->attack();
		}
	}

	// 끝난 공격 애니메이션 있나 시그널 확인
	//	m_eState = STAGEMONERSTATE::IDLE;
}

void CStageBoss::Late_Update(_float fTimeDelta)
{

}

HRESULT CStageBoss::Render()
{
	//렌더는 각 객체에서!
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
