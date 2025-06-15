#include "Damaged.h"
#include "GameInstance.h"

CDamaged::CDamaged(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CDamaged::CDamaged(const CDamaged& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CDamaged::Initialize()
{
    m_fSize = 6.f;
    m_fFrameOffsetTime = .06f;
    m_iChunk = 2;

    for (_uint i = 0; i < 3; i++)
    {
        PARTICLE attribute;
        m_Particles.push_back(attribute);
    }

    HRESULT hr = D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/Damaged.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 5;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CDamaged::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;
    
    attribute->_TextureIdx = 0;
    attribute->_position = *static_cast<_float3*>(pArg);
    attribute->_position.y += 1.f;
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    attribute->_size = d3d::GetRandomFloat(0.6f, 1.f);
}

void CDamaged::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    m_fDurationTime += fTimeDelta;

    if (m_fDurationTime > 2.f)
        return;

    for (auto& particle : m_Particles)
    {
        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= m_fFrameOffsetTime)
        {
            ++ particle._TextureIdx;
            particle._FrameTime = 0;
            if (particle._TextureIdx >= m_iNumTextures)
                ResetParticle(&particle, particle._position);
        }
    }

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

CDamaged* CDamaged::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CDamaged* pInstance = new CDamaged(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CDamaged"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CDamaged::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
