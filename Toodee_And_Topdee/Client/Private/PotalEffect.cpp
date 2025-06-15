#include "PotalEffect.h"
#include "GameInstance.h"

CPotalEffect::CPotalEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CPotalEffect::CPotalEffect(const CPotalEffect& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CPotalEffect::Initialize()
{
    m_fSize = 2.5f;
    m_fFrameOffsetTime = .025f;
    m_iChunk = 5.f;

    for (_uint i = 0; i < 40; i++)
    {
        PARTICLE attribute;
        m_Particles.push_back(attribute);
    }

    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/Twinkle.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
\
    m_iNumTextures = 10;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CPotalEffect::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;
    
    attribute->_TextureIdx = rand() % 10;
    _float3 pos = { 0.f,1.f,-10.f };
    _float3 min = { pos.x - 2.5f, pos.y, pos.z - 2.5f };
    _float3 max = { pos.x + 2.5f, pos.y, pos.z + 2.5f };


    d3d::GetRandomVector(&attribute->_position, &min, &max);
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    attribute->_size = d3d::GetRandomFloat(0.6f, 1.f);
}

void CPotalEffect::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    _uint count = 0;
    m_DurationTime += fTimeDelta;
    
    for (auto& particle : m_Particles)
    {
        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= m_fFrameOffsetTime)
        {
            ++ particle._TextureIdx;
            particle._FrameTime = 0;
            if (particle._TextureIdx >= m_iNumTextures)
            {
                particle._isAlive = false;
                if (m_DurationTime <= 2.5f && count < 10)
                {
                    ResetParticle(&particle);
                    ++ count;
                }
                else if (count < 1)
                {
                    ResetParticle(&particle);
                    ++count;
                }
            }
        }
    }   
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

CPotalEffect* CPotalEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPotalEffect* pInstance = new CPotalEffect(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CPotalEffect"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CPotalEffect::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
