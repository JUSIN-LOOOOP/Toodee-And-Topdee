#include "SemiclonDust.h"
#include "GameInstance.h"

CSemiclonDust::CSemiclonDust(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CSemiclonDust::CSemiclonDust(const CSemiclonDust& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CSemiclonDust::Initialize()
{
    m_fSize = 2.5f;
    m_iChunk = 1;

    for (_uint i = 0; i < 500; i++)
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

void CSemiclonDust::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    _float3 min = { pos.x + 4.f, pos.y - 8.f, pos.z - 4.f };
    _float3 max = { pos.x + 8.f, pos.y + 5.f, pos.z + 4.f };

    d3d::GetRandomVector(&attribute->_position, &min, &max);

    attribute->_size = d3d::GetRandomFloat(0.5f, 1.5f);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CSemiclonDust::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;
    for (auto& particle : m_Particles)
    {
        particle._size *= 0.95f;

        if (particle._size * m_fSize < 0.1f)
            particle._isAlive = false;

        particle._position.x += d3d::GetRandomFloat(0.02f, 0.07f);
    }   

}

CSemiclonDust* CSemiclonDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSemiclonDust* pInstance = new CSemiclonDust(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CSemiclonDust"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CSemiclonDust::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
