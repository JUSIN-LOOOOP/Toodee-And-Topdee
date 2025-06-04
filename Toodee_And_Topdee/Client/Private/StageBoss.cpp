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
	//����, ������, ���� ���� �Ŀ� Vector�� �־��ֱ�
	//������ �� this ������ �־��ֱ�
	//	->�̰ɷ� attack ������ �ñ׳� �ֱ�

	name = TEXT("StageBoss");

	return S_OK;
}

void CStageBoss::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down('X'))
	{
		m_eViewMode = (m_eViewMode == VIEWMODE::TOPDEE) ? VIEWMODE::TOODEE : VIEWMODE::TOPDEE;
		//�ñ׳� ������
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
	if(2D���)
		limbs[BODY]->2Dattack();
	
	else if (3D���)
		limbs[idx]->3Dattack();

		2D -> Head ���� && �ٸ� limb (hand)�� ����
		3D -> ��������

		�������� �̺�Ʈ
		1. ������ ��ȭ��		-> ȸ�� + HEAD������ �÷��׵� ���� (attack �ñ׳� ������ � attack�� �� �� Update�ȿ��� �б⹮ ó���Ϸ���)
		2. ���� ���

		�޾ƾ� �� �̺�Ʈ
		3. ������ ����

		�갡 �������ϴ� STATE�� ATTACK , IDLE 
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
			//limbs[BODY]->attack();	//���ؽñ׳�
			;
		else
		{
			_uint idx = rand() % ENUM_CLASS(LIMB::LIMB_END);
			//limbs[idx]->attack();
		}
	}

	// ���� ���� �ִϸ��̼� �ֳ� �ñ׳� Ȯ��
	//	m_eState = STAGEMONERSTATE::IDLE;
}

void CStageBoss::Late_Update(_float fTimeDelta)
{

}

HRESULT CStageBoss::Render()
{
	//������ �� ��ü����!
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
