#include "Wind.h"
#include "GameInstance.h"

CWind::CWind(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CWind::CWind(const CWind& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CWind::Initialize()
{
    BoundingBox box;

    box._max = _float3{ 140.f, -30.f, 80.f};
    box._min = _float3{- 150.f, - 30.f, -80.f};
    m_BoundingBox = box;
    m_fSize = 15.f;
    m_fSizeRatio = 0.12f;
    m_iChunk = 2;

    for (_uint i =  0; i < 10; i++)
        AddParticle();

    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/Wind2.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 8;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CWind::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;
    _float3 min = { -140.f, 30.f, - 50.f };
    _float3 max = _float3{ -60.f, 30.f, 50.f };
    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_position.y = m_BoundingBox._max.y;

     
    attribute->_velocity.x = d3d::GetRandomFloat(12.f, 35.f);
    attribute->_velocity.y = 0.0f;
    attribute->_velocity.z = d3d::GetRandomFloat(- 1.f, 2.f);

    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    attribute->_size = d3d::GetRandomFloat(0.3f, 1.2f);
    m_fFrameDuration = 0; 
}

void CWind::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._position += particle._velocity * fTimeDelta;
        m_fFrameDuration += fTimeDelta;
        particle._FrameTime += fTimeDelta;

        if (m_BoundingBox.isPointInside(particle._position) == false)
        {
            if (m_fFrameDuration > 5.f)
                ResetParticle(&particle);
            else
                particle._isAlive = false;
        }

        if (particle._FrameTime >= 0.1f)
        {
            particle._TextureIdx = (particle._TextureIdx + 1 >= m_iNumTextures) ? 0 : particle._TextureIdx + 1;
            particle._FrameTime = 0;
        }
    }
}

CWind* CWind::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWind* pInstance = new CWind(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CWind"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CWind::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
