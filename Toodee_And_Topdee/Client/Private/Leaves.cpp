#include "Leaves.h"
#include "GameInstance.h"

CLeaves::CLeaves(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CLeaves::CLeaves(const CLeaves& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CLeaves::Initialize()
{
    BoundingBox box;

    box._max = _float3{ 140.f, -30.f, 80.f};
    box._min = _float3{- 100.f, - 30.f, -80.f};
    m_BoundingBox = box;
    m_fSize = 2.f;
    vbSize = 2048;
    vbOffset = 0;
    vbBatchSize = 2;
    m_fFrameOffsetTime = .5f;

    for (_uint i =  0; i < 20; i++)
        AddParticle();

    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/theme2LeafSpr/Leaves.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));

    m_iNumTextures = 3;

    __super::Initialize();
    return S_OK;
}

void CLeaves::ResetParticle(PARTICLE* attribute)
{
    attribute->_isAlive = true;
    _float3 min = { 100.f, 30.f, 50.f };
    _float3 max = _float3{ 140.f, 30.f, 60.f };
    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_position.y = m_BoundingBox._max.y;

    attribute->_velocity.x = d3d::GetRandomFloat(0.0f, 2.5f) * -14.0f;
    attribute->_velocity.y =  0.0f;
    attribute->_velocity.z = d3d::GetRandomFloat(0.0f, 1.0f) * -10.0f;
    attribute->_TextureIdx = rand() % 3;

    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CLeaves::Update(_float fTimeDelta)
{
    for (auto& particle : m_Particles)
    {
        particle._position += particle._velocity * fTimeDelta;

        if (m_BoundingBox.isPointInside(particle._position) == false)
            ResetParticle(&particle);

        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= d3d::GetRandomFloat(0.05f, 0.1f))
        {
            particle._TextureIdx = (particle._TextureIdx + 1 >= m_iNumTextures) ? 0 : particle._TextureIdx + 1;
            particle._FrameTime = 0;

        }
    }   

}

CLeaves* CLeaves::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLeaves* pInstance = new CLeaves(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CLeaves"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CLeaves::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
