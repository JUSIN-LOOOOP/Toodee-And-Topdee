#include "BackWall.h"
#include "GameInstance.h"
#include "Util.h"

CBackWall::CBackWall(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CGameObject { pGraphic_Device }
{
} 

CBackWall::CBackWall(const CBackWall& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CBackWall::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackWall::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_iThemeIdx = *(static_cast<_uint*>(pArg));
    m_pTransformCom->Set_State(STATE::POSITION, { .5f, - 0.005f , 1.f });
    m_pTransformCom->Scaling((31.f * 2) - .5f, 17.f * 2, 1.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));    
    return S_OK;
}

void CBackWall::Priority_Update(_float fTimeDelta)
{

}

void CBackWall::Update(_float fTimeDelta)
{

}

void CBackWall::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBackWall::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(m_iThemeIdx))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBackWall::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BackWall"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;
    return S_OK;
}

CBackWall* CBackWall::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackWall* pInstance = new CBackWall(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBackWall"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBackWall* CBackWall::Clone(void* pArg)
{

    CBackWall* pInstance = new CBackWall(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBackWall"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackWall::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
