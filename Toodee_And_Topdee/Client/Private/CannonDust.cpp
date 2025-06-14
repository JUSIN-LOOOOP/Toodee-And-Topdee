#include "CannonDust.h"
#include "GameInstance.h"

CCannonDust::CCannonDust(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CCannonDust::CCannonDust(const CCannonDust& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CCannonDust::Initialize()
{
    m_fSize = 1.f;
    m_iChunk = 3;

    for (_uint i = 0; i < 20; i++)
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

void CCannonDust::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    _float3 min = { pos.x - .5f, pos.y - .5f, pos.z - .5f };
    _float3 max = { pos.x + .5f, pos.y + .5f, pos.z + .5f };

    d3d::GetRandomVector(&attribute->_position, &min, &max);

    attribute->_size = d3d::GetRandomFloat(0.8f, 1.2f);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    attribute->_position.z -= .3f;
    //attribute->_velocity = 
}

void CCannonDust::Update(_float fTimeDelta)
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

CCannonDust* CCannonDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCannonDust* pInstance = new CCannonDust(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CCannonDust"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CCannonDust::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
