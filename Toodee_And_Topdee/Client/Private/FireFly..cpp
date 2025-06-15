#include "FireFly.h"
#include "GameInstance.h"

CFireFly::CFireFly(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CFireFly::CFireFly(const CFireFly& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CFireFly::Initialize()
{
    BoundingBox box;
    box._max = _float3{ 40.f, 1.f, 30.f};
    box._min = _float3{ -40.f, 1.f, -30.f};
    m_BoundingBox = box;
    m_fSize = 1.f;
    m_iChunk = 15;
    m_fFrameOffsetTime = .5f;

    for (_uint i =  0; i < 20; i++)
        AddParticle();

    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/shineBiggerSpr_0.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));

    m_iNumTextures = 1;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}


void CFireFly::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    _uint posIdx = rand() % 4;
    _float3 min, max;


    switch (posIdx)
    {
    case 0:    //아래
        min = { -30.f, 1.f,  -20.f };
        max = { 30.f, 1.f, -20.f };
        break;
    case 1:    //위
        min = { -30.f, 1.f,  20.f };
        max = { 30.f, 1.f, 20.f };
        break;
    case 2:    //왼쪽
        min = { -30.f, 1.f,  -20.f };
        max = { -20.f, 1.f, 20.f };
        break;
    case 3:    //오른쪽
        min = { 20.f, 1.f,  -20.f };
        max = { 30.f, 1.f, 20.f };
        break;
    }    //}

    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_position.y = m_BoundingBox._max.y;

    attribute->_velocity.x = d3d::GetRandomFloat(-1.5f, 1.5f);
    attribute->_velocity.y = 0.f;
    attribute->_velocity.z = d3d::GetRandomFloat(-1.5f, 1.5f);

    attribute->_size = d3d::GetRandomFloat(0.5f, 1.5f);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CFireFly::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;
    for (auto& particle : m_Particles)
    {
        particle._position += particle._velocity * fTimeDelta * 0.5;

        if (m_BoundingBox.isPointInside(particle._position) == false)
            ResetParticle(&particle);

        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= d3d::GetRandomFloat(3.f, 8.f))
        {
            particle._velocity.x = d3d::GetRandomFloat(-2.5f, 2.5f) * 2.f;
            particle._velocity.z = d3d::GetRandomFloat(-2.5f, 2.5f) * 2.f;
            particle._FrameTime = 0;
        }
    } 
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

CFireFly* CFireFly::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFireFly* pInstance = new CFireFly(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CFireFly"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CFireFly::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
