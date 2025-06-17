#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject {pGraphic_Device}
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject {Prototype}
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

	Safe_Release(m_VIBufferCom_Diffuse);
	Safe_Release(m_pTargetTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	
}
