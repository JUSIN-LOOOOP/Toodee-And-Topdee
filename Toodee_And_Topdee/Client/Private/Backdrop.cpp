#include "Backdrop.h"
#include "GameInstance.h"
#include "Util.h"

CBackdrop::CBackdrop(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CGameObject { pGraphic_Device }
{
} 

CBackdrop::CBackdrop(const CBackdrop& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CBackdrop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackdrop::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_iThemeIdx = *(static_cast<_uint*>(pArg));
    m_pTransformCom->Set_State(STATE::POSITION, { 0.f, - .5f, 0.f });
    m_pTransformCom->Scaling(128.f * 0.7f, 72.f * 0.7f, 1.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

    //사이즈 설정하기
    return S_OK;
}

void CBackdrop::Priority_Update(_float fTimeDelta)
{

}

void CBackdrop::Update(_float fTimeDelta)
{

}

void CBackdrop::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CBackdrop::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(ENUM_CLASS(m_iThemeIdx))))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBackdrop::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Backdrop"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;
    return S_OK;
}

CBackdrop* CBackdrop::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBackdrop* pInstance = new CBackdrop(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBackdrop"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBackdrop* CBackdrop::Clone(void* pArg)
{

    CBackdrop* pInstance = new CBackdrop(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBackdrop"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackdrop::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
