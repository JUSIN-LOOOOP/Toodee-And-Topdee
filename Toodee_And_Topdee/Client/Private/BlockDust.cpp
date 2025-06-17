#include "BlockDust.h"
#include "GameInstance.h"

CBlockDust::CBlockDust(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CBlockDust::CBlockDust(const CBlockDust& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CBlockDust::Initialize()
{
    m_fSize = 3.f;
    m_fFrameOffsetTime = .06f;
    m_iChunk = 2;

    for (_uint i = 0; i < 100; i++)
    {
        PARTICLE attribute;
        m_Particles.push_back(attribute);
    }

    HRESULT hr = D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/BlockDust.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 9;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CBlockDust::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;
    
    attribute->_TextureIdx = 0;
    attribute->_position = *static_cast<_float3*>(pArg);
    attribute->_position.y += 1.f;
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CBlockDust::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= m_fFrameOffsetTime)
        {
            ++ particle._TextureIdx;
            particle._FrameTime = 0;
            if (particle._TextureIdx >= m_iNumTextures)
                particle._isAlive = false;
        }
    }   
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

CBlockDust* CBlockDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBlockDust* pInstance = new CBlockDust(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CBlockDust"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CBlockDust::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
