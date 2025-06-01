#include "PoolableObject.h"

CPoolableObject::CPoolableObject(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CGameObject{ pGraphic_Device }
{
}

CPoolableObject::CPoolableObject(const CPoolableObject& Prototype)
    :CGameObject{ Prototype }
{
}

void CPoolableObject::Free()
{
    __super::Free();
}
