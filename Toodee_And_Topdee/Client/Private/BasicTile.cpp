#include "BasicTile.h"
#include "GameInstance.h"
#include "Util.h"

CBasicTile::CBasicTile(LPDIRECT3DDEVICE9 pGraphic_Device) 
    : CTile{ pGraphic_Device }
{
} 

CBasicTile::CBasicTile(const CBasicTile& Prototype)
    : CTile{ Prototype }
{
}

HRESULT CBasicTile::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBasicTile::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_iTileSizeX = 2;
    m_iTileSizeY = 2;

    BLOCK_INFO* arg = static_cast<BLOCK_INFO*>(pArg);
    m_pTransformCom->Scaling(m_iTileSizeX, m_iTileSizeY, 2);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    m_pTransformCom->Set_State(STATE::POSITION, arg->vPos);
    m_tBlockInfo.iBlockType = arg->iBlockType;
    m_tBlockInfo.iDir = arg->iDir;
    m_tBlockInfo.iTextureIdx = arg->iTextureIdx;
    m_tBlockInfo.iTileTextureIdx = arg->iTileTextureIdx;

    return S_OK;
}

void CBasicTile::Priority_Update(_float fTimeDelta)
{
    if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
        Picking::Ray ray = Picking::GetRayFromMouse(g_iWinSizeX, g_iWinSizeY, g_hWnd, m_pGraphic_Device);

        if (S_OK == (IsTileClicked(ray)))
        {
            BLOCK_INFO newInfo = m_pGameInstance->Get_CurrentType();
            m_tBlockInfo.iTextureIdx = newInfo.iTextureIdx;
            m_tBlockInfo.iBlockType = newInfo.iBlockType;
            m_tBlockInfo.iDir = newInfo.iDir;
            m_tBlockInfo.iTileTextureIdx = newInfo.iTileTextureIdx;
            m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
            m_pTransformCom->TurnToRadian(_float3(0.f, 1.f, 0.f), D3DXToRadian(90.f) * m_tBlockInfo.iDir);
        }
    }
}

void CBasicTile::Update(_float fTimeDelta)
{

}

void CBasicTile::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CBasicTile::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_tBlockInfo.iTileTextureIdx)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBasicTile::Ready_Components()
{
    /* For.Com_VIBuffer*/
     if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_Component_Texture_Tile"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;
    return S_OK;
}

CBasicTile* CBasicTile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBasicTile* pInstance = new CBasicTile(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CBasicTile"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CBasicTile* CBasicTile::Clone(void* pArg)
{

    CBasicTile* pInstance = new CBasicTile(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CBasicTile"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBasicTile::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
