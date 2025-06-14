#include "ColorLight.h"
#include "GameInstance.h"

CColorLight::CColorLight(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem{ pGraphic_Device }
{
}

CColorLight::CColorLight(const CColorLight& Prototype)
    : CPSystem(Prototype)
{
}

HRESULT CColorLight::Initialize()
{
    BoundingBox box;

    box._max = _float3{ 140.f, -30.f, 80.f };
    box._min = _float3{ -100.f, -30.f, -80.f };
    m_BoundingBox = box;
    m_fSize = 30.f;
    m_iChunk = 4;

    for (_uint i = 0; i < 5; i++)
        AddParticle();

    //D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/ColorLightsAlpha.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/SpotColorLight.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));

    m_iNumTextures = 3;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}


void CColorLight::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;
    attribute->_TextureIdx = rand() % 3;
    _uint posIdx = rand() % 4;
    _float3 min, max;
    switch (posIdx)
    {
    case 0:    //�Ʒ�
        min = { -65.f, -30.f,  -40.f };
        max = { 65.f, -30.f, -35.f };
        break;
    case 1:    //��
        min = { -65.f, -30.f,  40.f };
        max = { 65.f, -30.f, 45.f };
        break;
    case 2:    //����
        min = { -65.f, -30.f,  -45.f };
        max = { -60.f, -30.f, 45.f };
        break;
    case 3:    //������
        min = { 60.f, -30.f,  -45.f };
        max = { 65.f, -30.f, 45.f };
        break;
    }
    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_position.y = m_BoundingBox._max.y;

    attribute->_velocity.x = d3d::GetRandomFloat(-2.5f, 2.5f) * 1.5f;
    attribute->_velocity.y = 0.f;
    attribute->_velocity.z = d3d::GetRandomFloat(-2.5f, 2.5f) * 1.5f;

    attribute->_size = d3d::GetRandomFloat(1.f, 2.5f);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CColorLight::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    m_fFrameDuration += fTimeDelta;
    
    for (auto& particle : m_Particles)
    {
        particle._position += particle._velocity * fTimeDelta;
        particle._FrameTime += fTimeDelta;

        if (m_BoundingBox.isPointInside(particle._position) == false)
            particle._isAlive = false;

        if (particle._isAlive == false && m_fFrameDuration > 10.f)
        {
            ResetParticle(&particle);
            m_fFrameDuration = 0;
        }

        if (particle._FrameTime >= d3d::GetRandomFloat(4.f, 8.f))
        {
            particle._velocity.x = d3d::GetRandomFloat(-2.5f, 2.5f) * 1.5f;
            particle._velocity.z = d3d::GetRandomFloat(-2.5f, 2.5f) * 1.5f;
            particle._FrameTime = 0;
        }
    }
}

CColorLight* CColorLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CColorLight* pInstance = new CColorLight(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CColorLight"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CColorLight::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
