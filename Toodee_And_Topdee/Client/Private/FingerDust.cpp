#include "FingerDust.h"
#include "GameInstance.h"

CFingerDust::CFingerDust(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CFingerDust::CFingerDust(const CFingerDust& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CFingerDust::Initialize()
{
    m_fSize = 5.f;
    m_iChunk = 20;

    for (_uint i = 0; i < 80; i++)
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

void CFingerDust::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    _float3 min = { pos.x - 12.f, pos.y - 10.f, pos.z - 12.f };
    _float3 max = { pos.x + 12.f, pos.y - 5.f, pos.z + 12.f };
    d3d::GetRandomVector(&attribute->_position, &min, &max);

    attribute->_size = d3d::GetRandomFloat(0.5f, 1.5f);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}   

void CFingerDust::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._size *= 0.9;

        if (particle._size * m_fSize < 0.1f)
            particle._isAlive = false;
    }
}

CFingerDust* CFingerDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFingerDust* pInstance = new CFingerDust(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CFingerDust"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CFingerDust::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
