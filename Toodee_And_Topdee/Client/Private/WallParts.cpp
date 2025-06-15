#include "WallParts.h"
#include "GameInstance.h"

CWallParts::CWallParts(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CPSystem { pGraphic_Device }
{
}

CWallParts::CWallParts(const CWallParts& Prototype)
    : CPSystem (Prototype)
{
}

HRESULT CWallParts::Initialize()
{
    m_fSize = .6f;
    m_fFrameOffsetTime = .5f;
    m_iChunk = 10;

    for (_uint i =  0; i < 200; i++)
    {
        PARTICLE attribute;
        m_Particles.push_back(attribute);
    }

    D3DXCreateTextureFromFile(m_pGraphic_Device, TEXT("../Resources/Textures/Particle/WallParts.png"), reinterpret_cast<LPDIRECT3DTEXTURE9*>(&m_Texture));
    m_iNumTextures = 3;

    __super::Initialize();
    __super::Stop();

    return S_OK;
}

void CWallParts::ResetParticle(PARTICLE* attribute, void* pArg)
{
    m_pTopdee = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(m_pGameInstance->Get_CurrentLevelID(), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0));
    m_pToodee = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(m_pGameInstance->Get_CurrentLevelID(), TEXT("Player_TooDee"), TEXT("Com_Transform"), 0));

    if (m_pTopdee == nullptr || m_pToodee == nullptr)
        return;

    attribute->_isAlive = true;

    _float3 pos = *static_cast<_float3*>(pArg);
    attribute->_BoundingBox._min = { pos.x - 1.5f, pos.y - 1.5f, pos.z - 1.f };
    attribute->_BoundingBox._max = { pos.x + 1.5f, pos.y + 1.5f, pos.z + 1.f };
    attribute->_BoundingBox._center = attribute->_BoundingBox._min + _float3( 1.5f, 1.5f, 1.f );

    d3d::GetRandomVector(&attribute->_position, &attribute->_BoundingBox._min, &attribute->_BoundingBox._max);

    attribute->_velocity.x = d3d::GetRandomFloat(0.0f, 1.5f);
    attribute->_velocity.y = d3d::GetRandomFloat(0.0f, 1.5f);
    attribute->_velocity.z = d3d::GetRandomFloat(0.0f, 1.5f);

    attribute->_size = d3d::GetRandomFloat(0.8f, 1.2f);
    attribute->_TextureIdx = rand() % 3;
    attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
}

void CWallParts::Update(_float fTimeDelta)
{
    if (isDead() == S_OK)
        return;

    for (auto& particle : m_Particles)
    {
        if (particle._isAlive == false)
            continue;

        _float3 TopdeeLength = m_pTopdee->Get_State(STATE::POSITION) - particle._position;
        _float3 ToodeeLength = m_pToodee->Get_State(STATE::POSITION) - particle._position;
        if ((D3DXVec3Length(&TopdeeLength) < 3.f && D3DXVec3Length(&TopdeeLength) > 1.5f) 
                || D3DXVec3Length(&ToodeeLength) < 3.f && D3DXVec3Length(&ToodeeLength) > 1.5f)
            particle._position -= (D3DXVec3Length(&TopdeeLength) < 3.f) ? *D3DXVec3Normalize(&TopdeeLength, &TopdeeLength) * fTimeDelta * 4.f : *D3DXVec3Normalize(&ToodeeLength, &ToodeeLength) * fTimeDelta * 4.f;
        else if (particle._BoundingBox.isPointInside(particle._position) == false || m_bOutFlag == true)  //밖에 있으면 돌아오게끔
        {
            m_bOutFlag = true;
            TopdeeLength = particle._BoundingBox._center - particle._position;
            if (D3DXVec3Length(&TopdeeLength) <= 5.f)
            {
                m_bOutFlag = false;
                particle._velocity *= -1.f;
            }
            particle._position += *D3DXVec3Normalize(&TopdeeLength, &TopdeeLength) * fTimeDelta * 2;
        }
        else
            particle._position += particle._velocity * fTimeDelta;
    }
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);

}

CWallParts* CWallParts::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWallParts* pInstance = new CWallParts(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CWallParts"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CWallParts::Free()
{
    __super::Free();
    Safe_Release(m_pGraphic_Device);
}
