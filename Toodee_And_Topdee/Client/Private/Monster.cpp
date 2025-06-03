#include "Monster.h"
#include "Transform.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject {pGraphic_Device}
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject {Prototype}
	, m_vFocusTargetPos{ Prototype.m_vFocusTargetPos }
{
}
HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = reinterpret_cast<MONSTER_DESC*>(pArg);

	m_pTargetTransformCom = pDesc->TargetTransformCom;

	Safe_AddRef(m_pTargetTransformCom);
	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{

}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
}

HRESULT CMonster::Render()
{
	

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransformCom);
	
}
