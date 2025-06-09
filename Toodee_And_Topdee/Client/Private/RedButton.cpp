#include "RedButton.h"
#include "GameInstance.h"
#include "Event.h"
CRedButton::CRedButton(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CRedButton::CRedButton(const CRedButton& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CRedButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRedButton::Initialize(void* pArg)
{ 
    _float3* pos = reinterpret_cast<_float3*>(pArg);
    m_vPosition_Face = *pos;

    m_vPosition_PressedButton = m_vPosition_Face;
    m_vPosition_PressedButton.z += 0.6f;

    m_vPosition_UnpressedButton = m_vPosition_Face;
    m_vPosition_UnpressedButton.z += 0.3f;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition_Face);
    m_pTransformCom->Scaling(2.f, 2.f, 2.f);

    m_iPlayLevel = m_pGameInstance->Get_CurrentLevelID();
    name = TEXT("REDBUTTON");

    return S_OK;
}

void CRedButton::Priority_Update(_float fTimeDelta)
{
}

void CRedButton::Update(_float fTimeDelta)
{
    m_pGameInstance->Check_Collision(m_pColliderCom);

    if (m_pColliderCom->OnCollisionEnter() || m_pColliderCom->OnCollisionStay())
        m_bPushed = true;

    if (m_pColliderCom->OnCollisionExit())
        m_bPushed = false;

    if (m_bPushed && m_bPreviousPushed != m_bPushed)
    {
        m_bPreviousPushed = true;
        EVENT_REDBUTTON_PRESSED Event;
        m_pGameInstance->Publish(m_iPlayLevel,EVENT_KEY::REDBUTTON_PRESSED, Event);
    }

    if (!m_bPushed && m_bPreviousPushed != m_bPushed)
    {
        m_bPreviousPushed = false;
        EVENT_REDBUTTON_PRESSED Event;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::REDBUTTON_UNPRESSED, Event);
    }
}

void CRedButton::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CRedButton::Render()
{
    SetUp_RenderState();
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    //Button
    if (m_bPushed)
    {
        m_pTransformCom->Scaling(1.7f, 1.f, 2.0f);
        m_pTransformCom->Set_State(STATE::POSITION, m_vPosition_PressedButton);
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pTextureCom_Button->Bind_Texture(1)))
            return E_FAIL;
    }
    else
    {
        m_pTransformCom->Set_State(STATE::POSITION, m_vPosition_UnpressedButton);
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pTextureCom_Button->Bind_Texture(0)))
            return E_FAIL;
    }
    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();
    m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    //Face
    m_pTransformCom->Set_State(STATE::POSITION, m_vPosition_Face);
 
    if (m_bPushed)
    {
        m_pTransformCom->Scaling(2.f, 2.f, 2.f);
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pTextureCom_Face->Bind_Texture(3)))
            return E_FAIL;
    }
    else
    {
        m_pTransformCom->Bind_Matrix();
        if (FAILED(m_pTextureCom_Face->Bind_Texture(0)))
            return E_FAIL;
    }
    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    Reset_RenderState();

    return S_OK;
}

HRESULT CRedButton::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_RedButton_ButtonFace"),
        TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Face))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_RedButton_ButtonPush"),
        TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Button))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 1.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Collider */
    CCollider::COLLIDER_DESC  ColliderDesc{};
    ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vColliderScale = _float3(2.f, 2.0f, 0.2f);
    ColliderDesc.vColliderPosion = _float3(m_vPosition_Face.x, m_vPosition_Face.y-2.f, m_vPosition_Face.z +2.1f);
    ColliderDesc.bIsFixed = true;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
    {
        MSG_BOX(TEXT("Failed to Add_Component : Com_Collider"));
        return E_FAIL;
    }

    return S_OK;
}

void CRedButton::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CRedButton::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}


CRedButton* CRedButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRedButton* pInstance = new CRedButton(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CRedButton"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRedButton::Clone(void* pArg)
{
    CRedButton* pInstance = new CRedButton(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CRedButton"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRedButton::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom_Face);
    Safe_Release(m_pTextureCom_Button);
}
