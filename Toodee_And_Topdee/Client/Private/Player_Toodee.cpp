#include "Player_Toodee.h"
#include "GameInstance.h"

CPlayer_Toodee::CPlayer_Toodee(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPlayer { pGraphic_Device }
{
}

CPlayer_Toodee::CPlayer_Toodee(const CPlayer_Toodee& Prototype)
    : CPlayer { Prototype }
    , m_fMaxJumpHeight { Prototype.m_fMaxJumpHeight }
{


}

HRESULT CPlayer_Toodee::Initialize_Prototype()
{
    m_iMaxAnimCount[ENUM_CLASS(PS_IDLE)] = 13;
    m_iMaxAnimCount[ENUM_CLASS(PS_MOVE)] = 11;
    m_iMaxAnimCount[ENUM_CLASS(PS_ACTION)] = 5;

    m_fAnimDelay = 0.05f;
    m_fMaxJumpHeight = 10.f;

    m_eCurrentDir = DIR_R;
    m_ePreDir = m_eCurrentDir;

    return S_OK;
}

HRESULT CPlayer_Toodee::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

    return S_OK;
}

void CPlayer_Toodee::Priority_Update(_float fTimeDelta)
{
}

void CPlayer_Toodee::Update(_float fTimeDelta)
{
    m_eCurrentState = PS_IDLE;

    if (m_pGameInstance->Key_Pressing(VK_RIGHT))
    {
        m_eCurrentState = PS_MOVE;
        m_eCurrentDir = DIR_R;
        m_pTransformCom->Go_Right(fTimeDelta);
    }

    if (m_pGameInstance->Key_Pressing(VK_LEFT))
    {
        m_eCurrentState = PS_MOVE;
        m_eCurrentDir = DIR_L;
        m_pTransformCom->Go_Right(fTimeDelta);
    }

    __super::Change_State();
    __super::Change_Dir();
}

void CPlayer_Toodee::Late_Update(_float fTimeDelta)
{
    /* 애니메이션 카운트 딜레이 */
    if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
    {
        m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount[ENUM_CLASS(m_eCurrentState)];
        m_fAnimTime = 0.f;
    }
    else
        m_fAnimTime += fTimeDelta;

    /* Topdee 시점에서 블렌딩 */
    if (m_eCurrentState == PS_STOP)
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
    else
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer_Toodee::Render()
{
    m_pTransformCom->Bind_Matrix();

    m_pTextureComs[ENUM_CLASS(m_eCurrentState)]->Bind_Texture(m_iCurrentAnimCount);

    m_pVIBufferCom->Bind_Buffers();

    Begin_RenderState();

    m_pVIBufferCom->Render();

    End_RenderState();

    return S_OK;
}

HRESULT CPlayer_Toodee::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if(FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;


    /* For.Com_Transform*/
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Texture*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Idle"),
        TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_IDLE)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Move"),
        TEXT("Com_Move_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_MOVE)]))))
        return E_FAIL;


    return S_OK;
}

HRESULT CPlayer_Toodee::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    if (m_eCurrentState == PS_STOP)
        m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    else
        m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}

HRESULT CPlayer_Toodee::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

CPlayer_Toodee* CPlayer_Toodee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayer_Toodee* pInstance = new CPlayer_Toodee(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CPlayer_Toodee"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayer_Toodee::Clone(void* pArg)
{
    CPlayer_Toodee* pInstance = new CPlayer_Toodee(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed Cloned : CPlayer_Toodee"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer_Toodee::Free()
{
    __super::Free();
}
