#include "ShotDust.h"
#include "GameInstance.h"

CShotDust::CShotDust(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CShotDust::CShotDust(const CShotDust& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CShotDust::Initialize()
{
    m_fSize = 4.f;
    m_iChunk = 15;

    for (_uint i = 0; i < 100; i++)
    {
        PARTICLE attribute;
        m_Particles.push_back(attribute);
    }

    HRESULT hr = D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/particleSpr.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 1;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CShotDust::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    _float3 min = { pos.x - 8.f, pos.y - 2.f, pos.z - 8.f };
    _float3 max = { pos.x + 8.f, pos.y - 5.f, pos.z + 8.f };
    d3d::GetRandomVector(&attribute->_position, &min, &max);

    attribute->_size = d3d::GetRandomFloat(0.5f, 1.5f);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}   

void CShotDust::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._size *= 0.9f;

        if (particle._size * m_fSize < 0.1f)
            particle._isAlive = false;
    }
}

CShotDust* CShotDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CShotDust* pInstance = new CShotDust(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CShotDust"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CShotDust::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
