#include "BackRock.h"
#include "GameInstance.h"
#include "Util.h"

CBackRock::CBackRock(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CBlendObject{ pGraphic_Device }
{
} 

CBackRock::CBackRock(const CBackRock& Prototype)
    : CBlendObject{ Prototype }
{
}

HRESULT CBackRock::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackRock::Initialize(void* pArg)
{
    BACK_DESC desc = *static_cast<BACK_DESC*>(pArg);

    m_Info._pos = desc._pos;
    m_Info._size = desc._size;
    m_Info._textureIdx = desc._textureIdx;
    m_fSpeed = m_pGameInstance->Rand(0.4f, 0.8f);
    m_fDir = (rand() % 2 == 0) ? 1.f : -1.f;
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, m_Info._pos);
    m_pTransformCom->Scaling(m_Info._size.x, m_Info._size.y, m_Info._size.z);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));    
    
    if (m_Info._textureIdx == 2)
        m_pTransformCom->TurnToRadian(_float3(0.f, 1.f, 0.f), -.2f);

    return S_OK;
}

void CBackRock::Priority_Update(_float fTimeDelta)
{
}

void CBackRock::Update(_float fTimeDelta)
{
    
    m_pTransformCom->Go_Up(fTimeDelta * m_fDir * m_fSpeed);
    m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * m_fDir * m_fSpeed);

    m_fMoveTime += fTimeDelta;
    if (m_fMoveTime >= m_pGameInstance->Rand(5.f, 15.f))
    {
        m_fDir *= -1;
        m_fMoveTime = 0;
    }
}

void CBackRock::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
    __super::Compute_CamDistance(m_pTransformCom);
}

HRESULT CBackRock::Render()
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

HRESULT CBackRock::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    return S_OK;
}

HRESULT CBackRock::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

HRESULT CBackRock::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
     _wstring strTag;
     switch ((m_Info._textureIdx))
     {
     case 0 :
         strTag = TEXT("Prototype_Component_Texture_BackRock");
         break;
     case 1: 
         strTag = TEXT("Prototype_Component_Texture_BackRock3");
         break;
     case 2:
         strTag = TEXT("Prototype_Component_Texture_Tree");
         break;
     }
     
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), strTag, TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC desc;
    desc.fRotationPerSec = 0.1f;
    desc.fSpeedPerSec = 1.f;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &desc)))
        return E_FAIL;
    return S_OK;
}

CBackRock* CBackRock::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackRock* pInstance = new CBackRock(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBackRock"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBackRock* CBackRock::Clone(void* pArg)
{

    CBackRock* pInstance = new CBackRock(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBackRock"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackRock::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
