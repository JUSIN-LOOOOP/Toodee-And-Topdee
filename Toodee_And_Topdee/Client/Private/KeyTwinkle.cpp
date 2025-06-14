#include "KeyTwinkle.h"
#include "GameInstance.h"

CKeyTwinkle::CKeyTwinkle(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CKeyTwinkle::CKeyTwinkle(const CKeyTwinkle& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CKeyTwinkle::Initialize()
{
    m_fSize = 2.f;
    m_iChunk = 1;
    m_fFrameOffsetTime = .02f;

    for (_uint i = 0; i < 20; i++)
    {
        PARTICLE attribute; 
        m_Particles.push_back(attribute);
    }

    HRESULT hr = D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/Twinkle.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 10;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CKeyTwinkle::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    _float3 min = { pos.x - .5f, 0, pos.z - .5f };
    _float3 max = { pos.x + .5f, 0, pos.z + .5f };

    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_TextureIdx = 0;
    attribute->_size = d3d::GetRandomFloat(0.6f, 1.f);

    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CKeyTwinkle::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= m_fFrameOffsetTime)
        {
            ++particle._TextureIdx;
            particle._FrameTime = 0;
            if (particle._TextureIdx >= m_iNumTextures)
                particle._isAlive = false;
        }
    }
}

CKeyTwinkle* CKeyTwinkle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CKeyTwinkle* pInstance = new CKeyTwinkle(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CKeyTwinkle"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CKeyTwinkle::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
