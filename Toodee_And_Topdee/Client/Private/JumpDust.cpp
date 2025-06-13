#include "JumpDust.h"
#include "GameInstance.h"

CJumpDust::CJumpDust(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CJumpDust::CJumpDust(const CJumpDust& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CJumpDust::Initialize()
{
    m_fSize = 2.f;
    m_iChunk = 5;

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

void CJumpDust::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    _float3 min = { pos.x - 1.2f, pos.y - 1.2f, pos.z - 1.2f };
    _float3 max = { pos.x + 1.2f, pos.y + 1.2f, pos.z + 1.2f };

    d3d::GetRandomVector(&attribute->_position, &min, &max);

    attribute->_position.z = pos.z - 1.f;

    attribute->_size = d3d::GetRandomFloat(0.8f, 1.4f);

    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CJumpDust::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;
    for (auto& particle : m_Particles)
    {
        particle._size *= 0.9;

        if (particle._size * m_fSize < 0.1f)
            particle._isAlive = false;

        particle._position.z += d3d::GetRandomFloat(0.02f, 0.07f);
    }   

}

CJumpDust* CJumpDust::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CJumpDust* pInstance = new CJumpDust(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CJumpDust"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CJumpDust::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
