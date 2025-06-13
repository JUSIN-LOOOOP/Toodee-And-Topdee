#include "Player_Toodee.h"
#include "GameInstance.h"
#include "PlayerState.h"
#include "Block.h"
#include "Event.h"
#include "Key.h"
#include "Level_Loading.h"

CPlayer_Toodee::CPlayer_Toodee(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CPlayer { pGraphic_Device }
{
}

CPlayer_Toodee::CPlayer_Toodee(const CPlayer_Toodee& Prototype)
    : CPlayer ( Prototype )
{


}

HRESULT CPlayer_Toodee::Initialize_Prototype()
{
    m_fPotalDistance = -5.f;    //임시

    m_bMoveInAction = true;     //Action 중 움직일수 있는가

    m_eActivateDimension = DIMENSION::TOODEE;

    m_eCurrentTextureDir = TEXTUREDIRECTION::RIGHT;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::IDLE)].eState = PLAYERSTATE::IDLE;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::IDLE)].iMaxAnimCount = 21;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::MOVE)].eState = PLAYERSTATE::MOVE;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::MOVE)].iMaxAnimCount = 12;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::ACTION)].eState = PLAYERSTATE::ACTION;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::ACTION)].iMaxAnimCount = 5;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::SWIM)].eState = PLAYERSTATE::SWIM;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::SWIM)].iMaxAnimCount = 8;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::STOP)].eState = PLAYERSTATE::STOP;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::STOP)].iMaxAnimCount = 46;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::CLEAR)].eState = PLAYERSTATE::CLEAR;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::CLEAR)].iMaxAnimCount = 17;

    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].eState = PLAYERSTATE::DEAD;
    m_tStateInitDesc[ENUM_CLASS(PLAYERSTATE::DEAD)].iMaxAnimCount = 5;

    return S_OK;
}

HRESULT CPlayer_Toodee::Initialize(void* pArg)
{
    m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_States()))
        return E_FAIL;

    if (FAILED(Ready_SubscribeEvent(m_iPlayLevel)))
        return E_FAIL;

    if (nullptr == pArg)
    {
        m_vPotalPosition = { 0.f, 0.f, 0.f };
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, 0.5f, 10.f));
    }
    else
    {
        BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

        _float3 vPosition = pDesc->vPos;
        vPosition.y += 1.f;
        m_pTransformCom->Set_State(STATE::POSITION, vPosition);
    }
  
    //Test true = 클리어모션 false = 플레이모션
    m_bCanClear = false;
    m_fCurrentJumpPower = 0.f;
    m_fStartJumpPower = 10.f;
    m_fAccumulationJumpPower = 0.f;
    m_fIncreaseJumpPower = 2.f;
    m_fMaxIncreaseJumpPower = 14.f; //임시
    m_fGravityPower = 0.f;

    m_pTransformCom->Scaling(12.f, 12.f, 0.f);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

    name = TEXT("Toodee");

    return S_OK;
}

void CPlayer_Toodee::Priority_Update(_float fTimeDelta)
{     
    Check_Dimension();

    m_pGroundCheckColliderCom->Collision_Off();


}

void CPlayer_Toodee::Update(_float fTimeDelta)
{
    if (m_eCurrentState != PLAYERSTATE::CLEAR)
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

            m_pGameInstance->Check_Collision(m_pColliderCom);
            Check_Collision();

            Check_Grounded();
        }

        m_pCurrentState->Update(this, fTimeDelta);

    }
    else
    {
        m_pCurrentState->Update(this, fTimeDelta);

        if (!m_bMoveToPotal)
        {
            if (m_pTransformCom->Approach(m_vPotalStartPosition, fTimeDelta, m_fClearSpeedPerSec))
            {
                m_bMoveToPotal = true;
                m_bClearAnimStart = true;
            }
        }

        if (m_bClearAnimStart)
        {
            if (m_pTransformCom->Spiral(m_vPotalPosition, _float3(0.f, 1.f, 0.f), 480.f, m_fPotalDistance, fTimeDelta))
            {
                m_bClear = true;

                if(LEVEL::LEVEL_MAPEDIT != static_cast<LEVEL>(m_iPlayLevel + 1))
                {
                    LEVELCHANGE_EVENT Event;
                    Event.iChangeLevel = m_iPlayLevel + 1;
                    Event.iCurrentLevel = m_iPlayLevel;

                    m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
                }
            }
        }
    }

}

void CPlayer_Toodee::Late_Update(_float fTimeDelta)
{
    /* 애니메이션 카운트 딜레이 */

    if (m_eCurrentState == PLAYERSTATE::STOP)
        m_iCurrentAnimCount = m_pPrevState->GetAnimCount();
    else
        m_iCurrentAnimCount = m_pCurrentState->GetAnimCount();
    
    if (m_eCurrentState == PLAYERSTATE::STOP)
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
    else if (m_eCurrentState == PLAYERSTATE::CLEAR)
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
    else
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer_Toodee::Render()
{
    if (FAILED(m_pColliderCom->Render()))
        return E_FAIL;

    if (FAILED(m_pGroundCheckColliderCom->Render()))
        return E_FAIL;
    
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

    m_pColliderCom->Collision_On();

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

void CPlayer_Toodee::Idle()
{
}

void CPlayer_Toodee::Move(_uint iInputData, _float fTimeDelta)
{
    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT) || iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT))
        m_pTransformCom->Go_Right(fTimeDelta);
}

void CPlayer_Toodee::Swim(_uint iInputData, _float fTimeDelta)
{
    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_UP))
        m_pTransformCom->Go_Up(fTimeDelta);

    if(iInputData & ENUM_CLASS(KEYINPUT::KEY_DOWN))
        m_pTransformCom->Go_Down(fTimeDelta);

    if (iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT) || iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT))
        m_pTransformCom->Go_Right(fTimeDelta);
}

void CPlayer_Toodee::Action()
{
    if (m_bInAction && m_eJumpState == JUMPSTATE::JUMPING && false == m_bOutWater) // 점프 중 이라면 점프 파워 상승
    {
        if (m_fAccumulationJumpPower + m_fIncreaseJumpPower <= m_fMaxIncreaseJumpPower)
        {
            m_fAccumulationJumpPower += m_fIncreaseJumpPower;
            m_fCurrentJumpPower += m_fIncreaseJumpPower;
        }
    }
    else if(!m_bInAction)   // 첫 점프
    {
        //Action 트리거 On
        m_bInAction = true;
        m_eJumpState = JUMPSTATE::JUMPING;
        //점프 최소 높이
        m_fCurrentJumpPower = m_fStartJumpPower;
        //초기화
        m_fGravityPower = 0.f;
        m_fAccumulationJumpPower = 0.f;
        _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
        m_pGameInstance->Set_Active(TEXT("Effect_Jump"), &pos);
    }
}

void CPlayer_Toodee::Stop()
{
    m_pColliderCom->Collision_Off();
   // m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);
}

void CPlayer_Toodee::Clear(_float3 vPortalPosition)
{
    m_bMoveToPotal = false;

    Change_TextureDir(TEXTUREDIRECTION::RIGHT);

    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

    m_vPotalStartPosition = { vPortalPosition.x + m_fPotalDistance, vPortalPosition.y, vPortalPosition.z };
  
    m_vPotalPosition = vPortalPosition;

    _float3 vSpeed = m_vPotalStartPosition - vPosition;

    m_fClearSpeedPerSec = D3DXVec3Length(&vSpeed);

    m_pColliderCom->Collision_Off();
}

void CPlayer_Toodee::Dead()
{
    LEVELCHANGE_EVENT Event;
    Event.iChangeLevel = m_iPlayLevel;
    Event.iCurrentLevel = m_iPlayLevel;
    m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
}

void CPlayer_Toodee::SwimEffect()
{
    m_pGameInstance->Set_Active(TEXT("Effect_Water"));
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

    if (m_eCurrentState == PLAYERSTATE::SWIM)
    {
        if (m_pGameInstance->Key_Pressing(VK_UP))
            iInputData |= ENUM_CLASS(KEYINPUT::KEY_UP);

        if (m_pGameInstance->Key_Pressing(VK_DOWN))
            iInputData |= ENUM_CLASS(KEYINPUT::KEY_DOWN);
    }

    return iInputData;
}


HRESULT CPlayer_Toodee::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

#pragma region Transform
    /* For.Com_Transform*/
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 12.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    CTransform::TRANSFORM_DESC		GroundCheckTransformDesc{};
    GroundCheckTransformDesc.fSpeedPerSec = 0.f;
    GroundCheckTransformDesc.fRotationPerSec = D3DXToRadian(90.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_GroundCheckTransform"), reinterpret_cast<CComponent**>(&m_pGroundCheckTransformCom), &GroundCheckTransformDesc)))
        return E_FAIL;

#pragma endregion

#pragma region Collider
    CCollider::COLLIDER_DESC PlayerColliderDesc{};
    PlayerColliderDesc.pOwner = this;
    PlayerColliderDesc.pTransform = m_pTransformCom;
    PlayerColliderDesc.vColliderScale = _float3(1.5f, 1.5f, 1.8f);
    PlayerColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
    PlayerColliderDesc.bIsFixed = false;
    m_fGroundCheckPosZ = (PlayerColliderDesc.vColliderScale.z * 0.6f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &PlayerColliderDesc)))
        return E_FAIL;

    CCollider::COLLIDER_DESC GroundCheckColliderDesc{};
    GroundCheckColliderDesc.pOwner = this;
    GroundCheckColliderDesc.pTransform = m_pGroundCheckTransformCom;
    GroundCheckColliderDesc.vColliderScale = _float3(1.f, 0.1f, 0.2f);
    GroundCheckColliderDesc.vColliderPosion = m_pGroundCheckTransformCom->Get_State(STATE::POSITION);
    GroundCheckColliderDesc.bIsFixed = false;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_GroundCheckCollider"), reinterpret_cast<CComponent**>(&m_pGroundCheckColliderCom), &GroundCheckColliderDesc)))
        return E_FAIL;

#pragma endregion

#pragma region Toodee Texture

   if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Idle"),
        TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::IDLE)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Move"),
        TEXT("Com_Move_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::MOVE)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Action"),
        TEXT("Com_Action_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::ACTION)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Swim"),
        TEXT("Com_Swim_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::SWIM)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Stop"),
        TEXT("Com_Stop_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::STOP)]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Clear"),
        TEXT("Com_Clear_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::CLEAR)]))))
        return E_FAIL;
  
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Toodee_Dead"),
        TEXT("Com_Dead_Texture"), reinterpret_cast<CComponent**>(&m_pTextureComs[ENUM_CLASS(PLAYERSTATE::DEAD)]))))
        return E_FAIL;

#pragma endregion

    return S_OK;
}

HRESULT CPlayer_Toodee::Ready_SubscribeEvent(_uint iPlayerLevel)
{
    m_pGameInstance->Subscribe<CANCLEAREVENT>(m_iPlayLevel, EVENT_KEY::CAN_CLEAR, [this](const CANCLEAREVENT& Event) {
        this->ClearReady(Event);
        });

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
    else if (m_eCurrentState == PLAYERSTATE::CLEAR)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
        m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
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
    else if (m_eCurrentState == PLAYERSTATE::CLEAR)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }
    else
        m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

void CPlayer_Toodee::Action_Jump(_float fTimeDelta)
{
    //중력 계산
   
    Compute_Gravity(fTimeDelta);

    //최고점에서 잠깐 머무르기
    if (m_fCurrentJumpPower < 0.f && m_eJumpState != JUMPSTATE::FALLING)
    {
        m_pCurrentState->UpdateAnim(fTimeDelta);
        m_eJumpState = static_cast<JUMPSTATE>(m_pCurrentState->GetAnimCount());
    }

    Gravity(fTimeDelta);
    
}

void CPlayer_Toodee::Gravity(_float fTimeDelta)
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

    vPosition.z += (m_fCurrentJumpPower * fTimeDelta);

    m_pTransformCom->Set_State(STATE::POSITION, vPosition);
}

void CPlayer_Toodee::Compute_Gravity(_float fTimeDelta)
{
    m_fGravityPower -= (GRAVITY * fTimeDelta) * 0.3f;

    if (m_fGravityPower <= -10.f)
        m_fGravityPower = -10.f;

    m_fCurrentJumpPower += m_fGravityPower;

    if (m_fCurrentJumpPower <= -20.f)
        m_fCurrentJumpPower = -20.f;
}

void CPlayer_Toodee::Check_Collision()
{
    if(m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
        m_bInWater = false;

    if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
    {

        vector<CGameObject*>* Overlaps = { nullptr };
        if (false == m_pColliderCom->GetOverlapAll(Overlaps))
            return;

        for (auto iter : *Overlaps)
        {
            Check_Collision_Dead(iter);
            Check_Collision_Portal(iter);
            Check_Collision_Key(iter);
            Check_Collision_Water(iter);
        }

        Check_Collision_PlayerState();
    }
    else
    {
        if (m_bEnterPortal)
        {
            m_bEnterPortal = false;
            EXITPORTALEVENT Event;
            Event.pPlayer = this;
            m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::EXIT_PORTAL, Event);
        }
    }
}

void CPlayer_Toodee::Check_Collision_PlayerState()
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

    //위치 보정
    _float3 temp;
    if (m_pColliderCom->GetCollisionsOffset(&temp))
    {
        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
        vPosition.x = vPosition.x + temp.x;
        vPosition.z = vPosition.z + temp.z;

        m_pTransformCom->Set_State(STATE::POSITION, vPosition);
    }

    COLLIDER_DIR eCollider_Dir = m_pColliderCom->DetectCollisionDirection();

    switch (eCollider_Dir)
    {
        case COLLIDER_DIR::FRONT:
        {
            if (m_pColliderCom->GetOverlapTarget()->Get_Name().find(TEXT("Key")) == std::string::npos
                && m_pColliderCom->GetOverlapTarget()->Get_Name().find(TEXT("Portal")) == std::string::npos)
                m_fCurrentJumpPower = 0.f;
            break;
        }
    }
}

void CPlayer_Toodee::Check_Collision_BlockBreak(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Break")) != string::npos)
    {
        COLLIDER_DIR eBreakCollider_Dir = m_pGroundCheckColliderCom->DetectCollisionDirection();
    
        if (eBreakCollider_Dir == COLLIDER_DIR::BACK)
        {
            BLOCKBREAKEVENT Event;
            Event.vPosition = m_pTransformCom->Get_State(STATE::POSITION);

            m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::BLOCK_BREAK, Event);
        }
    }
}

void CPlayer_Toodee::Check_Collision_Dead(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Enemy")) != string::npos)
    {
        m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::DEAD);
    }
}

void CPlayer_Toodee::Check_Collision_Portal(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Portal")) != string::npos)
    {
        m_bEnterPortal = true;
        m_bOnThePortal = true;
        ENTERPORTALEVENT Event;
        Event.pPlayer = this;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::ENTER_PORTAL, Event);
    }
    else
        m_bOnThePortal = false;
}

void CPlayer_Toodee::Check_Collision_Key(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Key")) != string::npos)
    {
        GETKEYEVENT Event;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::GET_KEY, Event);
        CKey* pKey = dynamic_cast<CKey*>(pGameObject);
        pKey->Get_Key();
    }
}

void CPlayer_Toodee::Check_Collision_Storm(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Storm")) != string::npos)
        m_bOnTheStorm = true;
    else
        m_bOnTheStorm = false;
    
}

void CPlayer_Toodee::Check_Collision_Water(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Water")) != string::npos)
    {
        m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::SWIM);
        m_bInWater = true;
        m_bInAction = false;
        m_bOutWater = false;
    }
}

void CPlayer_Toodee::Check_Grounded()
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
    vPosition = { vPosition.x, vPosition.y, vPosition.z - m_fGroundCheckPosZ };


    m_pGroundCheckTransformCom->Set_State(STATE::POSITION, vPosition);

    m_pGroundCheckColliderCom->Collision_On();

    m_pGameInstance->Check_Collision(m_pGroundCheckColliderCom);


    if (m_pGroundCheckColliderCom->OnCollisionEnter() || m_pGroundCheckColliderCom->OnCollisionStay())
    {
        vector<CGameObject*>* Overlaps = { nullptr };
        if (false == m_pGroundCheckColliderCom->GetOverlapAll(Overlaps))
            return;

        for (auto iter : *Overlaps)
        {
            Check_OnGround(iter);
            Check_Collision_Portal(iter);
            Check_Collision_BlockBreak(iter);
            Check_Collision_Dead(iter);
            Check_Collision_Key(iter);
            Check_Collision_Storm(iter);
        }

    }
    else
    {
        if (!m_bInAction)
        {
            m_bInAction = true;
            m_fGravityPower = 0.f;
            m_fCurrentJumpPower = -5.f;
            m_eJumpState = JUMPSTATE::JUMPING;
            m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::ACTION);
        }
    }
}

void CPlayer_Toodee::Check_OnGround(CGameObject* pGameObject)
{
    if (m_eJumpState != JUMPSTATE::JUMPING &&( pGameObject->Get_Name().find(TEXT("Wall")) != string::npos
        || pGameObject->Get_Name().find(TEXT("Block")) != string::npos))
    {
        m_bInAction = false;
        m_bOutWater = false;

        m_fAccumulationJumpPower = 0.f;
        m_fGravityPower = 0.f;
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

    Safe_Release(m_pGroundCheckColliderCom);
    Safe_Release(m_pGroundCheckTransformCom);
}
