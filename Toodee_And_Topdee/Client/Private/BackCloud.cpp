#include "BackCloud.h"
#include "GameInstance.h"
#include "Util.h"

CBackCloud::CBackCloud(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CBlendObject{ pGraphic_Device }
{
} 

CBackCloud::CBackCloud(const CBackCloud& Prototype)
    : CBlendObject{ Prototype }
{
}

HRESULT CBackCloud::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackCloud::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, {20.f, -.2f, - 8.f});
    m_pTransformCom->Scaling(40.f, 40.f, 1.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));    
    return S_OK;
}

void CBackCloud::Priority_Update(_float fTimeDelta)
{
}

void CBackCloud::Update(_float fTimeDelta)
{
    m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);

   
}

void CBackCloud::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
    __super::Compute_CamDistance(m_pTransformCom);
}

HRESULT CBackCloud::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(0))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    Begin_RenderState();

    m_pVIBufferCom->Render();

    End_RenderState();


    return S_OK;
}

HRESULT CBackCloud::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);    // 알파 테스트 ON
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);            // 기준 알파값 (0~255)
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 비교 방식
    return S_OK;
}

HRESULT CBackCloud::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    return S_OK;
}

HRESULT CBackCloud::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BackCloud"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC desc;
    desc.fRotationPerSec = 0.1f;
    //desc.fSpeedPerSec = 0.02f;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &desc)))
        return E_FAIL;
    return S_OK;
}

CBackCloud* CBackCloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackCloud* pInstance = new CBackCloud(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBackCloud"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBackCloud* CBackCloud::Clone(void* pArg)
{

    CBackCloud* pInstance = new CBackCloud(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBackCloud"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackCloud::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
