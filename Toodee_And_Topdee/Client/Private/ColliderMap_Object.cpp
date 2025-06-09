#include "ColliderMap_Object.h"
#include "GameInstance.h"


CColliderMap_Object::CColliderMap_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
    :   CGameObject{ pGraphic_Device }
{
}

CColliderMap_Object::CColliderMap_Object(const CColliderMap_Object& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CColliderMap_Object::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CColliderMap_Object::Initialize(void* pArg)
{
    COLLIDER_MAP_DESC* pDesc = static_cast<COLLIDER_MAP_DESC*>(pArg);
    
    m_vPosition = pDesc->vPosition;
    m_vScale = pDesc->vScale;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    m_pTransformCom->Scaling(m_vScale.x, m_vScale.y, m_vScale.z);

    name = TEXT("Wall");
    return S_OK;

}

void CColliderMap_Object::Priority_Update(_float fTimeDelta)
{
}

void CColliderMap_Object::Update(_float fTimeDelta)
{
}

void CColliderMap_Object::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CColliderMap_Object::Render()
{
    m_pColliderCom->Render();

    return S_OK;
}

_bool CColliderMap_Object::IsOverlappingWall(_float3 vOtherPosition, _float3 vLook)
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 vScale = m_pTransformCom->Get_Scaled();

    _float fMinX = vPosition.x - (vScale.x * 0.5f);
    _float fMaxX = vPosition.x + (vScale.x * 0.5f);
    _float fMinZ = vPosition.z - (vScale.z * 0.5f);
    _float fMaxZ = vPosition.z + (vScale.z * 0.5f);

    _float3 vCheckPosition = vOtherPosition + (vLook * 2.f);        // 2.f == TILESIZE

    if (vCheckPosition.x > fMinX && vCheckPosition.x < fMaxX)
        if (vCheckPosition.z > fMinZ && vCheckPosition.z < fMaxZ)
            return true;

    return false;
}


HRESULT CColliderMap_Object::Ready_Components()
{

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 0.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Collider */
    CCollider::COLLIDER_DESC  ColliderDesc{};
    ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vColliderPosion = m_vPosition;
    ColliderDesc.vColliderScale = m_vScale;
    ColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
    {
        MSG_BOX(TEXT("Failed to Add_Component : Com_Collider"));
        return E_FAIL;
    }

    return S_OK;
}

CColliderMap_Object* CColliderMap_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CColliderMap_Object* pInstance = new CColliderMap_Object(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CColliderMap_Object"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CColliderMap_Object* CColliderMap_Object::Clone(void* pArg)
{
    CColliderMap_Object* pInstance = new CColliderMap_Object(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CColliderMap_Object"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CColliderMap_Object::Free()
{
    __super::Free(); 

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pTransformCom);
}
