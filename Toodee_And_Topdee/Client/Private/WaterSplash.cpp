#include "WaterSplash.h"
#include "GameInstance.h"

CWaterSplash::CWaterSplash(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CWaterSplash::CWaterSplash(const CWaterSplash& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CWaterSplash::Initialize()
{
    m_fSize = 5.f;
    m_fFrameOffsetTime = .5f;
    m_iChunk = 1;

    for (_uint i = 0; i < 5; i++)
    {
        PARTICLE attribute;
        m_Particles.push_back(attribute);
    }

    HRESULT hr = D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/WaterSplash.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 10;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CWaterSplash::ResetParticle(PARTICLE* attribute, void* pArg)
{
    attribute->_isAlive = true;

    CTransform* Player = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(m_pGameInstance->Get_CurrentLevelID(), TEXT("Player_TooDee"), TEXT("Com_Transform"), 0));
    if (Player == nullptr)
        return;
    attribute->_TextureIdx = 0;
    attribute->_position = Player->Get_State(STATE::POSITION);
    attribute->_position.z += 1.5f;
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CWaterSplash::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        particle._FrameTime += fTimeDelta;
        if (particle._FrameTime >= .08f)
        {
            ++ particle._TextureIdx;
            particle._FrameTime = 0;
            if (particle._TextureIdx >= m_iNumTextures)
                particle._isAlive = false;
        }
    }   
}

CWaterSplash* CWaterSplash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWaterSplash* pInstance = new CWaterSplash(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CWaterSplash"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CWaterSplash::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
