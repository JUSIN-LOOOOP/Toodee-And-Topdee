#include "Block_Disappear.h"
#include "GameInstance.h"

CBlock_Disappear::CBlock_Disappear(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CBlock_Disappear::CBlock_Disappear(const CBlock_Disappear& Prototype)
    : CGameObject{ Prototype }

{
}

HRESULT CBlock_Disappear::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBlock_Disappear::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;
    
	_float3* pos = reinterpret_cast<_float3*>(pArg);
	m_vPosition = *pos;
    m_fPositionX[0] = m_vPosition.x - 2.f;
    m_fPositionX[1] = m_vPosition.x;
    m_fPositionX[2] = m_vPosition.x + 2.f;

    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    m_pTransformCom->Scaling(2.f, 2.f, 2.f);

    name = TEXT("Block_Disappear");

    m_iPlayLevel = m_pGameInstance->Get_CurrentLevelID();
    Ready_SubscribeEvent(m_iPlayLevel);

    return S_OK;
}

void CBlock_Disappear::Priority_Update(_float fTimeDelta)
{
}

void CBlock_Disappear::Update(_float fTimeDelta)
{
}

void CBlock_Disappear::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBlock_Disappear::Render()
{  
    return Block_Render();
}

void CBlock_Disappear::Event_Pressed(const EVENT_REDBUTTON_PRESSED& event)
{
    m_bActivity = false;
}

void CBlock_Disappear::Event_Unpressed(const EVENT_REDBUTTON_UNPRESSED& event)
{
    m_bActivity = true;
}

HRESULT CBlock_Disappear::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Disappear_Side"),
        TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Side))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Block_Disappear_Center"),
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Center))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 1.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Collision */
    CCollider::COLLIDER_DESC  ColliderDesc{};
    ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vColliderScale = _float3(6.f, 2.0f, 2.0f);
 //  ColliderDesc.vColliderPosion = m_vPosition;
    ColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
    {
        MSG_BOX(TEXT("Failed to Add_Component : Com_Collision"));
        return E_FAIL;
    }

    return S_OK;
}

void CBlock_Disappear::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBlock_Disappear::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

HRESULT CBlock_Disappear::Block_Render()
{
    if (!m_bActivity) return E_FAIL;

   SetUp_RenderState();

    //센터
   m_vPosition.x = m_fPositionX[1];
    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
    m_pTransformCom->Bind_Matrix();
    if (FAILED(m_pTextureCom_Center->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();
   
    //오른쪽
    m_vPosition.x = m_fPositionX[2];
    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    m_pTransformCom->Bind_Matrix();
    if (FAILED(m_pTextureCom_Side->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    //왼쪽
    m_vPosition.x = m_fPositionX[0];
   m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    m_pTransformCom->Rotation(_float3(0.f, 1.f, 0.f), D3DXToRadian(0.f));
    m_pTransformCom->Bind_Matrix();
    if (FAILED(m_pTextureCom_Side->Bind_Texture(0)))
        return E_FAIL;
    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    Reset_RenderState();

    m_vPosition.x = m_fPositionX[1];
    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition);
    return S_OK;
}

HRESULT CBlock_Disappear::Ready_SubscribeEvent(_uint iPlayerLevel)
{
    m_pGameInstance->Subscribe<EVENT_REDBUTTON_PRESSED>(iPlayerLevel, EVENT_KEY::REDBUTTON_PRESSED, [this](const EVENT_REDBUTTON_PRESSED& Event) {
        this->Event_Pressed(Event); });

    m_pGameInstance->Subscribe<EVENT_REDBUTTON_UNPRESSED>(iPlayerLevel, EVENT_KEY::REDBUTTON_UNPRESSED, [this](const EVENT_REDBUTTON_UNPRESSED& Event) {
        this->Event_Unpressed(Event); });

    return S_OK;
}


CBlock_Disappear* CBlock_Disappear::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBlock_Disappear* pInstance = new CBlock_Disappear(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBlock_Disappear"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBlock_Disappear::Clone(void* pArg)
{
    CBlock_Disappear* pInstance = new CBlock_Disappear(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBlock_Disappear"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBlock_Disappear::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom_Side);
    Safe_Release(m_pTextureCom_Center);

}
