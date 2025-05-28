#include "Tile.h"
#include "GameInstance.h"
#include "Util.h"

CTile::CTile(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CTile::CTile(const CTile& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CTile::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTile::Initialize(void* pArg)
{
    return S_OK;
}

void CTile::Priority_Update(_float fTimeDelta)
{

}

void CTile::Update(_float fTimeDelta)
{

}

void CTile::Late_Update(_float fTimeDelta)
{
}

HRESULT CTile::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iTextureType)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();

    m_pVIBufferCom->Render();

    return S_OK;
}

BLOCK_INFO CTile::Get_BlockInfo()
{
    BLOCK_INFO  rt;

    rt.vPos = m_pTransformCom->Get_State(STATE::POSITION);
    rt.vScale = m_pTransformCom->Get_Scaled();
    rt.iBlockType = m_iBlockType;
    rt.iTextureType = m_iTextureType;

    return rt;
}


HRESULT CTile::IsTileClicked(Picking::Ray ray)
{
    _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
    _float3 LH{ pos.x - (m_iTileSizeX / 2), pos.y, pos.z + (m_iTileSizeY / 2) };
    _float3 RH{ pos.x + (m_iTileSizeX / 2), pos.y, pos.z + (m_iTileSizeY / 2) };
    _float3 RB{ pos.x + (m_iTileSizeX / 2), pos.y, pos.z - (m_iTileSizeY / 2) };
    _float3 LB{ pos.x - (m_iTileSizeX / 2), pos.y, pos.z - (m_iTileSizeY / 2) };

    if (Picking::RayIntersectsTriangle(ray, LH, RH, RB, nullptr) == S_OK
        || Picking::RayIntersectsTriangle(ray, LH, RB, LB, nullptr) == S_OK)
        return S_OK;

    return E_FAIL;
}

void CTile::Set_Type(_int _newBlockType, _int _newTextureType)
{
    m_iBlockType = _newBlockType;
    m_iTextureType = _newTextureType;
}

void CTile::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
