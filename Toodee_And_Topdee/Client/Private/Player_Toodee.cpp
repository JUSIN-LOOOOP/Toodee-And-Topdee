#include "Player_Toodee.h"
#include "GameInstance.h"

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
    m_iMaxAnimCount[ENUM_CLASS(PS_IDLE)] = 13;
    m_iMaxAnimCount[ENUM_CLASS(PS_MOVE)] = 11;
    m_iMaxAnimCount[ENUM_CLASS(PS_ACTION)] = 5;
    m_iMaxAnimCount[ENUM_CLASS(PS_CLEAR)] = 17;

    m_fAnimDelay = 0.05f;
    //점프 최대 높이 조절
    m_fMaxJumpPower = 15.f; //임시
    //최고점 머무르는 시간 (= m_fHangDelay * 3 )
    m_fHangDelay = 0.05f;

    m_eCurrentDir = DIR_R;

    m_ePreDir = m_eCurrentDir;

    m_fPotalDistance = -5.f;



    return S_OK;
}

HRESULT CPlayer_Toodee::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(5.f, 5.f, 0.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    return S_OK;
}

void CPlayer_Toodee::Priority_Update(_float fTimeDelta)
{     //Test
    if(!m_bClear)
    {
        _float3 vTarget = { 0.f, 0.f, 0.f };
       __super::MoveToPotal(vTarget, _float3(0.f, 1.f, 0.f), fTimeDelta);
    }
}

void CPlayer_Toodee::Update(_float fTimeDelta)
{
    //점프 중에는 State Action으로 고정
    if(!m_bIsJumping && m_eCurrentState != PS_CLEAR)
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

    if (m_pGameInstance->Key_Pressing('Z'))
    {
        if(!m_bIsJumping)  
        {
            m_bIsJumping = true;
            m_eCurrentState = PS_ACTION;
            m_eJumpState = JS_JUMPING;
            //점프 최소 높이
            m_fCurrentJumpPower = 5.f;
            //초기화
            m_fGravityPower = 0.f;
            m_fHangTime = 0.f;
        }
        else
        {
            //점프 최대 높이 조절 ( 올라갈때만 증가 )
            if (m_fCurrentJumpPower + 2.f < m_fMaxJumpPower && m_eJumpState == JS_JUMPING)
                m_fCurrentJumpPower += 2.f;
        }
    }


    //점프 중 다른 State 변경 방지 + 점프 기능
    if (m_bIsJumping)
    {
        m_eCurrentState = PS_ACTION;
        Action_Jump(fTimeDelta);
    }

    __super::Change_State();
    __super::Change_Dir();

}

void CPlayer_Toodee::Late_Update(_float fTimeDelta)
{
    /* 애니메이션 카운트 딜레이 */
    if (m_bIsJumping)
    {
        m_iCurrentAnimCount = ENUM_CLASS(m_eJumpState);
    }
    else if (m_eCurrentState == PS_CLEAR)
    {
        if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
        {
            if (m_iCurrentAnimCount < m_iMaxAnimCount[ENUM_CLASS(PS_CLEAR)] - 1)
            {
                m_iCurrentAnimCount = m_iCurrentAnimCount++;
                m_fAnimTime = 0.f;
            }
        }
        else
            m_fAnimTime += fTimeDelta;
    }
    else
    {
        if (m_fAnimTime + fTimeDelta > m_fAnimDelay)
        {
            m_iCurrentAnimCount = (m_iCurrentAnimCount + 1) % m_iMaxAnimCount[ENUM_CLASS(m_eCurrentState)];
            m_fAnimTime = 0.f;
        }
        else
            m_fAnimTime += fTimeDelta;
    }
    
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

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Action"),
        TEXT("Com_Action_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_ACTION)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_Toodee_Clear"),
        TEXT("Com_Clear_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PS_CLEAR)]))))
        return E_FAIL;


    return S_OK;
}

HRESULT CPlayer_Toodee::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  //  m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  //  m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
  //  if (m_eCurrentState == PS_STOP)
  //  {
  //  }
  //  else
  //      m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  //
  //  m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  //  m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    return S_OK;
}

HRESULT CPlayer_Toodee::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    return S_OK;
}

void CPlayer_Toodee::Action_Jump(_float fTimeDelta)
{
    //중력 계산
    if (m_eJumpState == JS_FALLING || m_eJumpState == JS_JUMPING)
    {
        m_fGravityPower -= GRAVITY * fTimeDelta;
        
        m_fCurrentJumpPower += m_fGravityPower;
    }

    //떨어지는 속도 최대값 설정
    if (m_fCurrentJumpPower <= -15.f)
        m_fCurrentJumpPower = -15.f;

    //최고점에서 잠깐 머무르기
    if (m_fCurrentJumpPower * fTimeDelta <= 0.f && m_eJumpState != JS_FALLING)
    {
        if (m_eJumpState == JS_JUMPING)
            m_eJumpState = JS_HANGSTART;
        else if (m_eJumpState == JS_HANGEND)
            m_eJumpState = JS_FALLING;
        else
        {
            if (m_fHangTime + fTimeDelta > m_fHangDelay)
            {   
                m_eJumpState = static_cast<JUMPSTATE>(ENUM_CLASS(m_eJumpState) + 1);
                m_fHangTime = 0.f;
            }
            else
                m_fHangTime += fTimeDelta;
        }
    }

    //점프 높이 적용
    if(m_eJumpState == JS_FALLING || m_eJumpState == JS_JUMPING)
    {
        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

        vPosition.z += m_fCurrentJumpPower * fTimeDelta;

        //충돌 구현 후 변경
        if (vPosition.z <= 0.f)
        {
            m_bIsJumping = false;
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
