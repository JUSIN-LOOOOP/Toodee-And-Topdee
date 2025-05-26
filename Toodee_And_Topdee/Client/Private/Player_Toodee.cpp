#include "Player_Toodee.h"
#include "GameInstance.h"
#include "PlayerState.h"


CPlayer_Toodee::CPlayer_Toodee(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPlayer { pGraphic_Device }
{
}

CPlayer_Toodee::CPlayer_Toodee(const CPlayer_Toodee& Prototype)
    : CPlayer { Prototype }
    , m_fMaxJumpPower{ Prototype.m_fMaxJumpPower }
{


}

HRESULT CPlayer_Toodee::Initialize_Prototype()
{
    m_fMaxJumpPower = 15.f; //�ӽ�

    m_fPotalDistance = -5.f;    //�ӽ�

    m_bMoveInAction = true;     //Action �� �����ϼ� �ִ°�

    m_eActivateDimension = DIMENSION::TOODEE;

    m_eCurrentTextureDir = TEXTUREDIRECTION::RIGHT;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::IDLE)].eState = PLAYERSTATE::IDLE;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::IDLE)].iMaxAnimCount = 21;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::MOVE)].eState = PLAYERSTATE::MOVE;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::MOVE)].iMaxAnimCount = 12;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::ACTION)].eState = PLAYERSTATE::ACTION;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::ACTION)].iMaxAnimCount = 5;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::STOP)].eState = PLAYERSTATE::STOP;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::STOP)].iMaxAnimCount = 46;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::CLEAR)].eState = PLAYERSTATE::CLEAR;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::CLEAR)].iMaxAnimCount = 17;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].eState = PLAYERSTATE::DEAD;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].iMaxAnimCount = 0;

    return S_OK;
}

HRESULT CPlayer_Toodee::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_States()))
        return E_FAIL;

    //Test true = Ŭ������ false = �÷��̸��
    m_bCanClear = false;
    m_vPotalPosition = { 0.f, 0.f, 0.f };

    m_pTransformCom->Scaling(16.f, 16.f, 0.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    
    return S_OK;
}

void CPlayer_Toodee::Priority_Update(_float fTimeDelta)
{     
    Check_Dimension();
}

void CPlayer_Toodee::Update(_float fTimeDelta)
{
    if(!m_bCanClear)
    {
        if(m_eCurrentState != PLAYERSTATE::STOP)
        {
            _uint iInputData = KeyInput();

            m_pCurrentState->HandleInput(this, iInputData, fTimeDelta);

            ComputeTextureDirection(iInputData);

            if (m_bInAction)
            {
                Action_Jump(fTimeDelta);
            }
        }

        m_pCurrentState->Update(this, fTimeDelta);
    }
    else
    {
        if(!m_bMoveToPotal)
        {
            m_bMoveToPotal = m_pTransformCom->Move_To(m_vPotalStartPosition, fTimeDelta, m_fClearSpeedPerSec, 0.f);

            if (m_bMoveToPotal)
                m_bClearAnimStart = true;
        }

        if (m_bClearAnimStart)
        {
            m_pCurrentState->Update(this, fTimeDelta);

            if (m_pTransformCom->Spiral(m_vPotalPosition, _float3(0.f,1.f,0.f), 480.f, m_fPotalDistance, fTimeDelta))
            {
                m_bClear = true;
            }
        }
    }
}

void CPlayer_Toodee::Late_Update(_float fTimeDelta)
{
    /* �ִϸ��̼� ī��Ʈ ������ */

    if (m_eCurrentState == PLAYERSTATE::STOP)
        m_iCurrentAnimCount = m_pPrevState->GetAnimCount();
    else
        m_iCurrentAnimCount = m_pCurrentState->GetAnimCount();
    
    if (m_eCurrentState == PLAYERSTATE::STOP)
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
    else
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer_Toodee::Render()
{
    m_pTransformCom->Bind_Matrix();
    
    if (m_eCurrentState == PLAYERSTATE::STOP)
    {
        m_pTextureComs[ENUM_CLASS(m_eCurrentState)]->Bind_Texture(m_iStopAnimCount);    //Stop OutLine Texxture
    }
    else
        m_pTextureComs[ENUM_CLASS(m_eCurrentState)]->Bind_Texture(m_iCurrentAnimCount);
    
    m_pVIBufferCom->Bind_Buffers();

    Begin_RenderState();

    m_pVIBufferCom->Render();

    End_RenderState();

    return S_OK;
}

HRESULT CPlayer_Toodee::Return_PrevState()
{
    if (m_pPrevState == nullptr)
        return E_FAIL;

    if (m_pCurrentState)
    {
        m_pCurrentState->Exit(this);
        Safe_Release(m_pCurrentState);
    }

    m_pCurrentState = m_pPrevState;
    m_eCurrentState = m_pPrevState->GetTextureKey();

    Safe_Release(m_pPrevState);

    Safe_AddRef(m_pCurrentState);

    if (m_pPrevState)
        m_pPrevState = nullptr;

    return S_OK;
}

void CPlayer_Toodee::Move(_float fTimeDelta)
{
    m_pTransformCom->Go_Right(fTimeDelta);
}

void CPlayer_Toodee::Action()
{
    if (m_bInAction) // ���� �� �̶�� ���� �Ŀ� ���
    {
        if (m_fCurrentJumpPower + 2.f < m_fMaxJumpPower && m_eJumpState == JUMPSTATE::JUMPING)
            m_fCurrentJumpPower += 2.f;

        return;
    }
    else    // ù ����
    {
        //Action Ʈ���� On
        m_bInAction = true;
        m_eJumpState = JUMPSTATE::JUMPING;
        //���� �ּ� ����
        m_fCurrentJumpPower = 5.f;
        //�ʱ�ȭ
        m_fGravityPower = 0.f;
    }
}

void CPlayer_Toodee::Stop()
{
    m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);
}


_uint CPlayer_Toodee::KeyInput()
{
    _uint iInputData = 0;

    if (m_pGameInstance->Key_Pressing(VK_LEFT))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_LEFT);

    if(m_pGameInstance->Key_Pressing(VK_RIGHT))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_RIGHT);

    if (m_pGameInstance->Key_Pressing('Z'))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_Z);

    if (m_pGameInstance->Key_Down('X'))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_X);

    return iInputData;
}


HRESULT CPlayer_Toodee::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform*/
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Idle"),
        TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::IDLE)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Move"),
        TEXT("Com_Move_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::MOVE)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Action"),
        TEXT("Com_Action_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::ACTION)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Stop"),
        TEXT("Com_Stop_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::STOP)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Clear"),
        TEXT("Com_Clear_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::CLEAR)]))))
        return E_FAIL;


    return S_OK;
}

HRESULT CPlayer_Toodee::Ready_States()
{
    /* For State State_Idle */
    for (_uint i = 0; i < ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END); i++)
    {
        PLAYERSTATE eState = m_tStateInitDesc[i].eState;

        if(FAILED(__super::Add_State(eState, &m_tStateInitDesc[i])))
            return E_FAIL;
    }

    m_pCurrentState = Find_State(PLAYERSTATE::IDLE);

    if (!m_pCurrentState)
        return E_FAIL;

    Safe_AddRef(m_pCurrentState);

    return S_OK;
}

HRESULT CPlayer_Toodee::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (m_eCurrentState == PLAYERSTATE::STOP)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    }
    else
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    }

    return S_OK;
}

HRESULT CPlayer_Toodee::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    if (m_eCurrentState == PLAYERSTATE::STOP)
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    else
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

void CPlayer_Toodee::Action_Jump(_float fTimeDelta)
{
    //�߷� ���
    if (m_eJumpState == JUMPSTATE::FALLING || m_eJumpState == JUMPSTATE::JUMPING)
    {
        m_fGravityPower -= GRAVITY * fTimeDelta;
        
        m_fCurrentJumpPower += m_fGravityPower;
    }

    //�������� �ӵ� �ִ밪 ����
    if (m_fCurrentJumpPower <= -15.f)
        m_fCurrentJumpPower = -15.f;

    //�ְ������� ��� �ӹ�����
    if (m_fCurrentJumpPower * fTimeDelta <= 0.f && m_eJumpState != JUMPSTATE::FALLING)
    {
        m_pCurrentState->UpdateAnim(fTimeDelta);
        m_eJumpState = static_cast<JUMPSTATE>(m_pCurrentState->GetAnimCount());
    }

    //���� ���� ����
    if(m_eJumpState == JUMPSTATE::FALLING || m_eJumpState == JUMPSTATE::JUMPING)
    {
        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

        vPosition.z += m_fCurrentJumpPower * fTimeDelta;

        //�浹 ���� �� ����
        if (vPosition.z <= 0.f)
        {
            m_bInAction = false;
            vPosition.z = 0.f;
        }

        m_pTransformCom->Set_State(STATE::POSITION, vPosition);
    }
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
