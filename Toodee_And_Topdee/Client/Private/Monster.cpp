#include "Monster.h"
#include "Transform.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject {pGraphic_Device}
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject {Prototype}
	, m_fMoveX{ Prototype.m_fMoveX }
	, m_fMoveY{ Prototype.m_fMoveY }
	, m_fMoveZ{ Prototype.m_fMoveZ }
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

void CMonster::Move_To_Target(CTransform* pTransform, _float fTimeDelta)
{
	m_vFocusTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	
	pTransform->Move_To(m_vFocusTargetPos, fTimeDelta / 2.f);

	
	
	/*_float3 vMoveDelta{};
	
	D3DXVec3Normalize(&vMoveDelta, &m_vDelta);
	
	m_fMoveX = vMoveDelta.x;
	m_fMoveY = vMoveDelta.y;
	m_fMoveZ = vMoveDelta.z;*/
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransformCom);
	
}
