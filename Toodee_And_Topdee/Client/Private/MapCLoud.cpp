#include "MapCloud.h"
#include "GameInstance.h"

CMapCloud::CMapCloud(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CMapCloud::CMapCloud(const CMapCloud& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CMapCloud::Initialize()
{
    BoundingBox box;

    box._max = _float3{ 140.f, -30.f, 80.f};
    box._min = _float3{- 150.f, - 30.f, -80.f};
    m_BoundingBox = box;
    m_fSize = 14.f;
    m_iChunk =10;

    for (_uint i =  0; i < 20; i++)
        AddParticle();

    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/Cloud.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 1;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CMapCloud::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;
    _float3 min = { -140.f, 30.f, - 50.f };
    _float3 max = _float3{ -60.f, 30.f, 50.f };
    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_position.y = m_BoundingBox._min.y;

    m_fSizeRatio = 0.5f;
    attribute->_velocity.x = d3d::GetRandomFloat(10.f, 25.f);
    attribute->_velocity.y = 0.0f;
    attribute->_velocity.z = 0.0f;
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    attribute->_size = d3d::GetRandomFloat(0.8f, 1.2f);
    m_fFrameDuration = 0; 
}

void CMapCloud::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._position += particle._velocity * fTimeDelta;
        m_fFrameDuration += fTimeDelta;
        particle._FrameTime += fTimeDelta;

        if (m_BoundingBox.isPointInside(particle._position) == false)
            if (m_fFrameDuration > 5.f)
                ResetParticle(&particle);
    }
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

}

CMapCloud* CMapCloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMapCloud* pInstance = new CMapCloud(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CMapCloud"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CMapCloud::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
