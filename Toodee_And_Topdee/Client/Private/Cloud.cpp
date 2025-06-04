#include "Cloud.h"
#include "GameInstance.h"


CCloud::CCloud(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CCloud::CCloud(const CCloud& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CCloud::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CCloud::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    name = TEXT("Cloud");

    m_iMotionNumber = {0};
    m_fIntervalMotion = {0.25f};
    m_fAccumulateMotion = {0.f};

    m_ToodeePosition = { 25.f, 1.9f, 9.f };
    m_TopdeePosition = { 23.f, 5.f, 9.f };


    if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
    {
        m_pTransformCom->Set_State(STATE::POSITION, m_ToodeePosition);
        m_eDimension = DIMENSION::TOODEE;
    }
    else
    {
        m_pTransformCom->Set_State(STATE::POSITION, m_TopdeePosition);
        m_eDimension = DIMENSION::TOPDEE;

    }

    m_pTransformCom->Scaling(9.f, 4.f, 1.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90));
   


    return S_OK;
}

void CCloud::Priority_Update(_float fTimeDelta)
{
}

void CCloud::Update(_float fTimeDelta)
{
    Motion(fTimeDelta);
    if (!m_bIsChangeCamera && m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
        m_bIsChangeCamera = true;
   
    PositionChangeForCameraSwitch(fTimeDelta);
}

void CCloud::Late_Update(_float fTimeDelta)
{

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CCloud::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iMotionNumber)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    SetUp_RenderState();

    m_pVIBufferCom->Render();

    Reset_RenderState();

    m_pColliderCom->Render();

    return S_OK;
}



void CCloud::Motion(_float fTimeDelta)
{
    if (m_fIntervalMotion < m_fAccumulateMotion + fTimeDelta)
    {
        m_fAccumulateMotion = 0.f;
        m_iMotionNumber = m_iMotionNumber + 1 == 6 ? 0 : m_iMotionNumber + 1;
    }
    else
        m_fAccumulateMotion += fTimeDelta;
    
}

void CCloud::PositionChangeForCameraSwitch(_float fTimeDelta)
{
    if (!m_bIsChangeCamera) return;

	if (m_eDimension == DIMENSION::TOODEE)
	{
		_float3 vTemp = m_pTransformCom->Get_State(STATE::POSITION);

        if (vTemp.y <= m_TopdeePosition.y) vTemp.y += fTimeDelta * 5.5f;
        else vTemp.y = m_TopdeePosition.y;

        if (vTemp.x >= m_TopdeePosition.x) vTemp.x -= fTimeDelta * 1.f ;
        else vTemp.x = m_TopdeePosition.x;

		m_pTransformCom->Set_State(STATE::POSITION, vTemp);
	}
	else
	{
		_float3 vTemp = m_pTransformCom->Get_State(STATE::POSITION);

		if (vTemp.y >= m_ToodeePosition.y) vTemp.y -= fTimeDelta * 5.5f;
        else  vTemp.y = m_ToodeePosition.y;

        if (vTemp.x <= m_ToodeePosition.x) vTemp.x += fTimeDelta * 1.f;
        else vTemp.x = m_ToodeePosition.x;

		m_pTransformCom->Set_State(STATE::POSITION, vTemp);
	}

    if (m_pGameInstance->Get_CurrentDimension() != DIMENSION::CHANGE)
    {
        m_bIsChangeCamera = false;
        m_eDimension = m_eDimension == DIMENSION::TOODEE ? DIMENSION::TOPDEE : DIMENSION::TOODEE;
    }
}


HRESULT CCloud::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Cloud"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
        return E_FAIL;

    /* For.Com_Collision */
    CCollider::COLLIDER_DESC  ColliderDesc{};
    ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
    ColliderDesc.pTransform = m_pTransformCom;
    ColliderDesc.vColliderScale = _float3(7.f, 3.0f, 2.0f);
    ColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
    {
        MSG_BOX(TEXT("Failed to Add_Component : Com_Collision"));
        return E_FAIL;
    }

    return S_OK;
}

void CCloud::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

void CCloud::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

CCloud* CCloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCloud* pInstance = new CCloud(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CCloud"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCloud::Clone(void* pArg)
{
    CCloud* pInstance = new CCloud(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CCloud"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCloud::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
