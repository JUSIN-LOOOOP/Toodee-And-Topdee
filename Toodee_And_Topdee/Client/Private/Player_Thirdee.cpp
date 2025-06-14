#include "Player_Thirdee.h"
#include "GameInstance.h"

#include "Thirdee_Idle.h"
#include "Thirdee_Move.h"
#include "Thirdee_Action.h"
#include "Thirdee_Dead.h"
#include "Thirdee_Clear.h"

#include "Thirdee_Body.h"
#include "InteractionBlock.h"
#include "ColliderMap_Object.h"
#include "Key.h"

CPlayer_Thirdee::CPlayer_Thirdee(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CPlayer_Thirdee::CPlayer_Thirdee(const CPlayer_Thirdee& Prototype)
    : CGameObject { Prototype }
{
}

HRESULT CPlayer_Thirdee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Thirdee::Initialize(void* pArg)
{
#pragma region LIGHT
    D3DLIGHT9			LightDesc{};
    LightDesc.Type = D3DLIGHT_DIRECTIONAL;
    LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    LightDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
    LightDesc.Direction = _float3(1.f, 0.f, 1.f);
    m_pGraphic_Device->SetLight(0, &LightDesc);

    D3DMATERIAL9		MtrlDesc{};
    MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    MtrlDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    m_pGraphic_Device->SetMaterial(&MtrlDesc);

    m_pGraphic_Device->LightEnable(0, true);
#pragma endregion

    m_iPlayLevel = m_pGameInstance->Get_NextLevelID();

    if (FAILED(Ready_Bodys()))
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    if (FAILED(Ready_States()))
        return E_FAIL;

 
#pragma region START SETTING
    name = TEXT("Player");

    BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

    _float3 vPosition = pDesc->vPos;
    vPosition.y += 1.f;
    m_pTransformCom->Set_State(STATE::POSITION, vPosition);

    m_pTransformCom->TurnToRadian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-90.f));


    m_eCurrentDimension = DIMENSION::TOPDEE;
    m_ePrevDimension = DIMENSION::TOPDEE;
    
    m_RootWorldMatrix = *m_pTransformCom->Get_WorldMatrix();

    m_vLookPoint = vPosition;
    m_vLookPoint.z -= 1.f;

    Topdee_Check_Center();

    m_bFalling = false;
    m_fCurrentGravity = 0.f;
    m_fCurrentJumpPower = 0.f;
    m_fMaxGravity = 10.f;
    m_fAccumulationJumpPower = 0.f;
#pragma endregion

	return S_OK;
}

void CPlayer_Thirdee::Priority_Update(_float fTimeDelta)
{
    Check_Dimension(fTimeDelta);
    m_pRootPart->Priority_Update(fTimeDelta);
}

void CPlayer_Thirdee::Update(_float fTimeDelta)
{
    _uint iInputData = KeyInput();
    
    if (m_eCurrentState == PLAYERSTATE::DEAD || m_eCurrentState == PLAYERSTATE::CLEAR)
    {
        m_pCurrentState->Update(this, fTimeDelta);
    }
    else
    {
        if (m_eCurrentDimension == DIMENSION::TOPDEE)
        {
            Check_Topdee_State(iInputData, fTimeDelta);
        }
        else if (m_eCurrentDimension == DIMENSION::TOODEE)
        {
            Check_Toodee_State(iInputData, fTimeDelta);
        }
    }

    m_RootWorldMatrix = *m_pTransformCom->Get_WorldMatrix();
    m_pRootPart->Set_ParentWorldMatrix(m_RootWorldMatrix);
    m_pRootPart->AnimUpdate(fTimeDelta, m_eCurrentState);
    m_pRootPart->Update(fTimeDelta);
}

void CPlayer_Thirdee::Late_Update(_float fTimeDelta)
{
    m_pRootPart->Late_Update(fTimeDelta);
}

HRESULT CPlayer_Thirdee::Render()
{
    m_pGroundCheckColliderCom->Render();

    Begin_RenderState();

    m_pRootPart->Render();

    End_RenderState();

	return S_OK;
}

void CPlayer_Thirdee::EnterIdle()
{
    Topdee_Check_Center();
    m_bInAction = false;
}

void CPlayer_Thirdee::Idle(_float fTimeDelta)
{
    if (m_eCurrentDimension == DIMENSION::TOPDEE)
        m_pTransformCom->Approach(m_vTilePoint, fTimeDelta, 5.f);
}

void CPlayer_Thirdee::EnterMove()
{
    Topdee_Check_Center();
}

void CPlayer_Thirdee::Move(_float fTimeDelta)
{
    if (m_eCurrentDimension == DIMENSION::TOPDEE && false == m_bInAction)
        Topdee_Move(fTimeDelta);
    else if (m_eCurrentDimension == DIMENSION::TOODEE)
        Toodee_Move(fTimeDelta);
}

void CPlayer_Thirdee::EnterAction()
{
    m_bInAction = true;

    Topdee_Check_Center();

    if (m_eCurrentDimension == DIMENSION::TOODEE)
    {
        m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
        m_pGameInstance->PlayAudio(TEXT("PlayerJump.wav"), CHANNELID::SOUND_PLAYER, 0.5f);
        m_fCurrentJumpPower = 10.f;
        m_fCurrentGravity = 0.f;
        m_fAccumulationJumpPower = 0.f;
        m_bJumpStart = true;
    }
}

void CPlayer_Thirdee::Action(_float fTimeDelta)
{
    if (m_eCurrentDimension == DIMENSION::TOPDEE)
    {
        Topdee_Action(fTimeDelta);
    }
    else if (m_eCurrentDimension == DIMENSION::TOODEE)
    {
        Toodee_Action(fTimeDelta);
    }
}

void CPlayer_Thirdee::Dead(_float fTimeDelta)
{
    _float3 vLook = m_pTransformCom->Get_State(STATE::LOOK);
    D3DXVec3Normalize(&vLook, &vLook);

    if (m_pTransformCom->Turn_Divide(vLook, D3DXToRadian(-90.f), D3DXToRadian(90.f), fTimeDelta))
    {
        LEVELCHANGE_EVENT Event;
        Event.iChangeLevel = ENUM_CLASS(LEVEL::LEVEL_FINALBOSS1);
        Event.iCurrentLevel = m_iPlayLevel;

        m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
    }
}

void CPlayer_Thirdee::EnterClear()
{
    m_pColliderCom->Collision_Off();
}

void CPlayer_Thirdee::Clear(_float fTimeDelta)
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

    vPosition.y += 5.f * fTimeDelta;

    m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);

    if (vPosition.y >= 10.f)
    {
        LEVELCHANGE_EVENT Event;
        Event.iCurrentLevel = m_iPlayLevel;
        Event.iChangeLevel = m_iPlayLevel + 1;
        m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::CHANGE_LEVEL, Event);
    }

    m_pTransformCom->Set_State(STATE::POSITION, vPosition);
}

HRESULT CPlayer_Thirdee::Change_State(PLAYERSTATE eNewState)
{
    if (m_eCurrentState == eNewState)
        return S_OK;

    if (m_pCurrentState)
    {
        m_pCurrentState->Exit(this);
        
        Safe_Release(m_pCurrentState);
    }

    auto iterState = m_States.find(eNewState);
    if (iterState == m_States.end())
        return E_FAIL;

    m_pCurrentState = iterState->second;
    m_eCurrentState = eNewState;

    m_pCurrentState->Enter(this);
    Safe_AddRef(m_pCurrentState);

    return S_OK;
}


HRESULT CPlayer_Thirdee::Ready_Component()
{
    CTransform::TRANSFORM_DESC TransformDesc{};
    TransformDesc.fSpeedPerSec = 12.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(720.f);

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    CCollider::COLLIDER_DESC ColliderDesc{};
    ColliderDesc.pOwner = this;
    ColliderDesc.bIsFixed = false;
    ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
    ColliderDesc.vColliderScale = _float3(1.5f, 2.1f, 1.5f);
    ColliderDesc.pTransform = m_pTransformCom;
    
    m_fGroundCheckColliderY = ColliderDesc.vColliderScale.z * 0.7f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    CCollider::COLLIDER_DESC GroundCheckColliderDesc{};
    GroundCheckColliderDesc.pOwner = this;
    GroundCheckColliderDesc.bIsFixed = false;
    GroundCheckColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
    GroundCheckColliderDesc.vColliderScale = _float3(1.5f, 1.5f, 0.2f);
    GroundCheckColliderDesc.pTransform = m_pTransformCom;

    m_fGroundCheckColliderY += GroundCheckColliderDesc.vColliderScale.z * 0.5f;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_GroundCheckCollider"), reinterpret_cast<CComponent**>(&m_pGroundCheckColliderCom), &GroundCheckColliderDesc)))
        return E_FAIL;

    CCollider::COLLIDER_DESC AttachCheck_ColliderDesc{};
    AttachCheck_ColliderDesc.pOwner = this;
    AttachCheck_ColliderDesc.bIsFixed = false;
    AttachCheck_ColliderDesc.vColliderPosion = m_pTransformCom->Get_State(STATE::POSITION);
    AttachCheck_ColliderDesc.vColliderScale = _float3(3.5f, 3.5f, 3.5f);
    AttachCheck_ColliderDesc.pTransform = m_pTransformCom;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_AttachCheckCollider"), reinterpret_cast<CComponent**>(&m_pAttachCheckColliderCom), &AttachCheck_ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer_Thirdee::Ready_Bodys()
{
    _float4x4 LocalMatrix;

    D3DXMatrixIdentity(&LocalMatrix);

    CThirdee_Part::THIRDEEPART_DESC BodyPartDesc{};

    BodyPartDesc.LocalMatrix = LocalMatrix;
    BodyPartDesc.vLocalTransform = _float3(0.f, 0.f, 0.f);
    BodyPartDesc.vScale = _float3(0.3f, 0.3f, 0.4f);
    BodyPartDesc.vRotationRadian = _float3(0.f, 0.f, 0.f);
    BodyPartDesc.strTextureTag = TEXT("Prototype_Component_Texture_Thirdee_Body");
    BodyPartDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Capsule");
    BodyPartDesc.eType = PART_TYPE::BODY;

    m_pRootPart = CThirdee_Body::Create(m_pGraphic_Device, &BodyPartDesc);

    return S_OK;
}

HRESULT CPlayer_Thirdee::Ready_States()
{
    Add_State(PLAYERSTATE::IDLE);
    Add_State(PLAYERSTATE::MOVE);
    Add_State(PLAYERSTATE::ACTION);
    Add_State(PLAYERSTATE::DEAD);
    Add_State(PLAYERSTATE::CLEAR);

    EnterIdle();
    m_pCurrentState = Find_State(PLAYERSTATE::IDLE);

    if (!m_pCurrentState)
        return E_FAIL;

    Safe_AddRef(m_pCurrentState);

    return S_OK;
    return S_OK;
}

HRESULT CPlayer_Thirdee::Begin_RenderState()
{
    m_pGraphic_Device->LightEnable(0, true);
  //  m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  //  m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
  //  m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CPlayer_Thirdee::End_RenderState()
{
    m_pGraphic_Device->LightEnable(0, false);
 //   m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CThirdee_State* CPlayer_Thirdee::Find_State(PLAYERSTATE eKeyState)
{
    auto iter = m_States.find(eKeyState);

    if (iter == m_States.end())
        return nullptr;

    return iter->second;
}

HRESULT CPlayer_Thirdee::Add_State(PLAYERSTATE eKeyState)
{
    if (Find_State(eKeyState) != nullptr)
        return E_FAIL;

    CThirdee_State* pInstance = nullptr;

    switch (eKeyState)
    {
    case PLAYERSTATE::IDLE:
        pInstance = CThirdee_Idle::Create(eKeyState);
        break;
    case PLAYERSTATE::MOVE:
        pInstance = CThirdee_Move::Create(eKeyState);
        break;
    case PLAYERSTATE::ACTION:
        pInstance = CThirdee_Action::Create(eKeyState);
        break;
    case PLAYERSTATE::DEAD:
        pInstance = CThirdee_Dead::Create(eKeyState);
        break;
    case PLAYERSTATE::CLEAR:
        pInstance = CThirdee_Clear::Create(eKeyState);
        break;
    }

    m_States.emplace(eKeyState, pInstance);

    return S_OK;
}

_uint CPlayer_Thirdee::KeyInput()
{
    _uint iInputData = 0;

    if(m_eCurrentDimension == DIMENSION::TOPDEE)
    {
        if (m_pGameInstance->Key_Pressing(VK_LEFT))
            iInputData |= ENUM_CLASS(KEYINPUT::KEY_LEFT);

        if (m_pGameInstance->Key_Pressing(VK_RIGHT))
            iInputData |= ENUM_CLASS(KEYINPUT::KEY_RIGHT);
    }

    if (m_pGameInstance->Key_Pressing(VK_UP))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_UP);

    if (m_pGameInstance->Key_Pressing(VK_DOWN))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_DOWN);

    if (m_pGameInstance->Key_Pressing('Z'))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_Z);

    if (m_pGameInstance->Key_Down('X'))
        iInputData |= ENUM_CLASS(KEYINPUT::KEY_X);

    return iInputData;
}

void CPlayer_Thirdee::Check_Dimension(_float fTimeDelta)
{    
    DIMENSION eNewDimension = m_pGameInstance->Get_CurrentDimension();

    if (m_eCurrentDimension == eNewDimension
        && m_ePrevDimension == eNewDimension)
        return;

    m_eCurrentDimension = m_pGameInstance->Get_CurrentDimension();

    switch (m_ePrevDimension)
    {
    case DIMENSION::TOODEE:
        if(m_pTransformCom->Turn_Divide(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f), D3DXToRadian(90.f), fTimeDelta))
            m_ePrevDimension = m_eCurrentDimension;
        break;
    case DIMENSION::TOPDEE:
        if (m_pTransformCom->Turn_Divide(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f), D3DXToRadian(90.f), fTimeDelta))
            m_ePrevDimension = m_eCurrentDimension;

        if (m_pAttachBlock)
        {
            Check_Collision_Detach();
            m_pAttachBlock->Request_Change_State(BLOCKSTATE::DETACH);
            m_pAttachBlock->Detach(m_vDetachPosition, 20.f);
            m_pAttachBlock = nullptr;
        }

        break;
    case DIMENSION::CHANGE:
        m_ePrevDimension = m_eCurrentDimension;
        break;
    }


    EnterIdle();
    m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::IDLE);
}

void CPlayer_Thirdee::Gravity(_float fTimeDelta)
{
   
        m_fCurrentGravity += GRAVITY * fTimeDelta * 0.3f;

        if (m_fCurrentGravity >= m_fMaxGravity)
            m_fCurrentGravity = m_fMaxGravity;

        m_fCurrentJumpPower -= m_fCurrentGravity;

        if (m_fCurrentJumpPower < 0.f)
            m_bJumpStart = false;

        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

        vPosition.z += m_fCurrentJumpPower * fTimeDelta;

        m_pTransformCom->Set_State(STATE::POSITION, vPosition);
   
}

void CPlayer_Thirdee::Check_Collision_PlayerPos()
{
    _float3 temp;
    if (m_pColliderCom->GetCollisionsOffset(&temp))
    {
        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
        vPosition.x = vPosition.x + temp.x;
        vPosition.z = vPosition.z + temp.z;

        m_pTransformCom->Set_State(STATE::POSITION, vPosition);
    }
}

void CPlayer_Thirdee::Check_Collision_Enemy(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Enemy")) != string::npos)
    {

        m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::DEAD);
    }
}

void CPlayer_Thirdee::Check_Collision_Portal(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Portal")) != string::npos)
    {
        EnterClear();
        m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::CLEAR);
    }
}

void CPlayer_Thirdee::Check_Collision_Key(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Key")) != string::npos)
    {
        CKey* pKey = dynamic_cast<CKey*>(pGameObject);
        pKey->Get_Key();
        GETKEYEVENT Event;
        m_pGameInstance->Publish(m_iPlayLevel, EVENT_KEY::GET_KEY, Event);

    }
}

void CPlayer_Thirdee::Check_Topdee_State(_uint iInputData, _float fTimeDelta)
{
    Topdee_Direction(iInputData, fTimeDelta);    
    m_pCurrentState->HandleInput(this, iInputData, fTimeDelta);
    m_pCurrentState->Update(this, fTimeDelta);
    Check_Topdee_Collision();
    Interaction();

}

void CPlayer_Thirdee::Check_Topdee_Collision()
{
    m_pGameInstance->Check_Collision(m_pColliderCom);

    if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
    {
        vector<CGameObject*>* Overlaps = { nullptr };
        if (false == m_pColliderCom->GetOverlapAll(Overlaps))
            return;

        for (auto iter : *Overlaps)
        {
            Check_Collision_InteractionBlock(iter);
            Check_Collision_Enemy(iter);
            Check_Collision_Portal(iter);
        }

        Check_Collision_PlayerPos();
    }
}

void CPlayer_Thirdee::Check_Collision_InteractionBlock(CGameObject* pGameObject)
{
    if (pGameObject->Get_Name().find(TEXT("Interaction")) != wstring::npos)
    {
        CInteractionBlock* pBlock = dynamic_cast<CInteractionBlock*>(pGameObject);
        if (false == pBlock->IsPush() && false == pBlock->IsFall())
        {
            pBlock->Request_Change_State(BLOCKSTATE::PUSH);
            m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
            m_pGameInstance->PlayAudio(TEXT("BlockPush.wav"), CHANNELID::SOUND_EFFECT, 0.5f);

            pBlock->Push(m_ePushDirZ, m_ePushDirX, 8.f);
        }
    }
}
void CPlayer_Thirdee::Check_Collision_Attach()
{
    _float fMaxDot = -2.f;

    m_pAttachCheckColliderCom->Collision_On();

    m_pGameInstance->Check_Collision(m_pAttachCheckColliderCom);

    if (m_pAttachCheckColliderCom->OnCollisionStay() || m_pAttachCheckColliderCom->OnCollisionEnter())
    {
        vector<CGameObject*>* Overlaps = { nullptr };
        m_pAttachCheckColliderCom->GetOverlapAll(Overlaps);

        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
        _float3 vLook = m_vLookDir;

        for (auto iter : *Overlaps)
        {
            if ((iter->Get_Name().find(TEXT("Interaction")) != string::npos))
            {
                CInteractionBlock* pBlock = dynamic_cast<CInteractionBlock*>(iter);

                _float fDot = pBlock->ComputeDirDotLook(vPosition, vLook);

                if (fDot > fMaxDot)
                {
                    fMaxDot = fDot;
                    m_pFocusBlock = pBlock;
                }
            }
        }
    }
}
void CPlayer_Thirdee::Check_Collision_Detach()
{
    _float fMaxDotByDir[4];

    for (_uint i = 0; i < 4; i++)
    {
        fMaxDotByDir[i] = -2.f;
    }

    _float fMaxDot = -2.f;

    // LEFT, UP, RIGHT, DOWN 방향 Check 용

    _float3 Dir[4] = { {-1.f, 0.f, 0.f},
                        { 0.f, 0.f, 1.f},
                        { 1.f, 0.f, 0.f },
                        { 0.f, 0.f, -1.f} };

    m_pAttachCheckColliderCom->Collision_On();

    m_pGameInstance->Check_Collision(m_pAttachCheckColliderCom);

    if (m_pAttachCheckColliderCom->OnCollisionStay() || m_pAttachCheckColliderCom->OnCollisionEnter())
    {
        vector<CGameObject*>* Overlaps = { nullptr };
        m_pAttachCheckColliderCom->GetOverlapAll(Overlaps);

        _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
        _float3 vLook = m_vLookDir;

        for (auto iter : *Overlaps)
        {
            if ((iter->Get_Name().find(TEXT("Block")) != string::npos))
            {
                CBlock* pBlock = dynamic_cast<CBlock*>(iter);

                _float fDot = pBlock->ComputeDirDotLook(vPosition, vLook);

                if (fDot > fMaxDot)
                {
                    fMaxDot = fDot;
                }
       
                for (_uint i = 0; i < 4; i++)
                {
                    _float fDotByDir = pBlock->ComputeDirDotLook(vPosition, Dir[i]);

                    if (fDotByDir > fMaxDotByDir[i])
                    {
                        fMaxDotByDir[i] = fDotByDir;
                    }
                }
                
            }
            else if (iter->Get_Name().find(TEXT("Wall")) != string::npos)
            {
                CColliderMap_Object* pWall = dynamic_cast<CColliderMap_Object*>(iter);

                _float fResult = static_cast<_float>(pWall->IsOverlappingWall(vPosition, vLook));

                if (fResult > fMaxDot)
                {
                    fMaxDot = fResult;
                }

                for (_uint i = 0; i < 4; i++)
                {
                    _float fDotByDir = static_cast<_float>(pWall->IsOverlappingWall(vPosition, Dir[i]));

                    if (fDotByDir > fMaxDotByDir[i])
                    {
                        fMaxDotByDir[i] = fDotByDir;
                    }
                }
            }
        }
    }

    if (fMaxDot <= 0.8f) // 오차 범위 감안
    {
        m_vDetachPosition = m_vTilePoint + m_vDirPoint;
        m_bCanDetach = true;
        return;
    }
    else
    {
        for (_uint i = 0; i < ENUM_CLASS(DETACHDIRECTION::DETACHDIRECTION_END); i++)
        {
            if (fMaxDotByDir[i] <= 0.8f) // 오차 범위 감안
            {
                m_vDetachPosition = Compute_Detach_Position(static_cast<DETACHDIRECTION>(i));
                m_bCanDetach = true;
                return;
            }
        }
    }

    m_bCanDetach = false;
}

_float3 CPlayer_Thirdee::Compute_Detach_Position(DETACHDIRECTION eDir)
{
    
    _float3 vCenter = m_vTilePoint;

    const _float TILE_SIZE = 2.f;

    _float fDistanceX = 0.f;
    _float fDistanceZ = 0.f;

    switch (eDir)
    {
    case DETACHDIRECTION::LEFT:
        fDistanceX = -2.f;
        break;
    case DETACHDIRECTION::UP:
        fDistanceZ = 2.f;
        break;
    case DETACHDIRECTION::RIGHT:
        fDistanceX = 2.f;
        break;
    case DETACHDIRECTION::DOWN:
        fDistanceZ = -2.f;
        break;
    }

    _float3 vDetachPosition = { vCenter.x + fDistanceX, vCenter.y, vCenter.z + fDistanceZ };
    return vDetachPosition;
}

void CPlayer_Thirdee::Interaction()
{
    if (m_pGameInstance->Key_Up('Z'))
    {
        if (m_pAttachBlock && true == m_bCanDetach)
        {
            m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
            m_pGameInstance->PlayAudio(TEXT("BlockDetach.wav"), CHANNELID::SOUND_PLAYER, 0.5f);

            m_pAttachBlock->Request_Change_State(BLOCKSTATE::DETACH);
            m_pAttachBlock->Detach(m_vDetachPosition, 20.f);
            m_pAttachBlock = nullptr;
        }
        else
        {
            if (m_pFocusBlock)
            {
                if (m_pFocusBlock->IsStop())
                {
                    m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
                    m_pGameInstance->PlayAudio(TEXT("BlockAttach.wav"), CHANNELID::SOUND_PLAYER, 0.5f);

                    m_pFocusBlock->Request_Change_State(BLOCKSTATE::ATTACH);
                    m_pFocusBlock->Attach(m_pTransformCom, 20.f);
                    m_pAttachBlock = m_pFocusBlock;
                }
                m_pFocusBlock = nullptr;
            }
        }
        m_bInAction = false;
    }
}

void CPlayer_Thirdee::Topdee_Check_Center()
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

    const _float TILE_SIZE = 2.f;

    _int iCountX = static_cast<_int>(floorf(vPosition.x / TILE_SIZE));
    _int iCountZ = static_cast<_int>(floorf(vPosition.z / TILE_SIZE));

    _float fCenterX = {};
    _float fCenterZ = {};

    fCenterX = static_cast<_float>(iCountX) * TILE_SIZE + static_cast<_float>(TILE_SIZE * 0.5);
    fCenterZ = static_cast<_float>(iCountZ) * TILE_SIZE + static_cast<_float>(TILE_SIZE * 0.5);

    m_vTilePoint = { fCenterX, vPosition.y, fCenterZ };
}

void CPlayer_Thirdee::Compute_PushDirection(_float3 vMovePos)
{
    if (vMovePos.z > 0.f)
        m_ePushDirZ = MOVEDIRECTION::UP;
    else if (vMovePos.z < 0.f)
        m_ePushDirZ = MOVEDIRECTION::DOWN;
    else
        m_ePushDirZ = MOVEDIRECTION::TRANSVERSE;

    if (vMovePos.x > 0.f)
        m_ePushDirX = TEXTUREDIRECTION::RIGHT;
    else if (vMovePos.x < 0.f)
        m_ePushDirX = TEXTUREDIRECTION::LEFT;
}

void CPlayer_Thirdee::Topdee_Direction(_uint iInputData, _float fTimeDelta)
{
    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        _float3 vLook = m_pTransformCom->Get_State(STATE::POSITION);

        m_vDirPoint = { 0.f,0.f,0.f };
        m_vLookDir = { 0.f, 0.f, 0.f };

        if(m_eCurrentDimension == DIMENSION::TOPDEE)
        {
            if (iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT))
            {
                vLook.x += 1.f;
                m_vLookDir.z += 1.f;
                m_vDirPoint.z += 2.f;
            }
            if (iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT))
            {
                vLook.x -= 1.f;
                m_vLookDir.z -= 1.f;
                m_vDirPoint.z -= 2.f;
            }
        }

        if (iInputData & ENUM_CLASS(KEYINPUT::KEY_UP))
        {
            vLook.z -= 1.f;
            m_vLookDir.x += 1.f;
            m_vDirPoint.x += 2.f;
        }
        if (iInputData & ENUM_CLASS(KEYINPUT::KEY_DOWN))
        {
            vLook.z += 1.f;
            m_vLookDir.x -= 1.f;
            m_vDirPoint.x -= 2.f;
        }

        if (D3DXVec3Length(&m_vDirPoint) <= 0.0001)
            return;

        m_vLookPoint = vLook;
        m_vMovePoint = m_pTransformCom->Get_State(STATE::POSITION) + m_vDirPoint;
        Compute_PushDirection(m_vDirPoint);
    }

    m_pTransformCom->Look_At_Divide(m_vLookPoint, fTimeDelta);
}

void CPlayer_Thirdee::Topdee_Move(_float fTimeDelta)
{
    if(m_pTransformCom->Approach(m_vMovePoint, fTimeDelta, 12.f))
    {
        Topdee_Check_Center();
    }
}

void CPlayer_Thirdee::Topdee_Action(_float fTimeDelta)
{
    m_pTransformCom->Approach(m_vTilePoint, fTimeDelta, 5.f);

    if (nullptr == m_pAttachBlock)
        Check_Collision_Attach();
    else
        Check_Collision_Detach();
}

void CPlayer_Thirdee::Check_Toodee_State(_uint iInputData, _float fTimeDelta)
{
    Toodee_Direction(iInputData, fTimeDelta);    
    m_pCurrentState->HandleInput(this, iInputData, fTimeDelta);
    m_pCurrentState->Update(this, fTimeDelta);
    Check_Toodee_Collision();
    Gravity(fTimeDelta);
}

void CPlayer_Thirdee::Check_Toodee_Collision()
{
    m_pGameInstance->Check_Collision(m_pColliderCom);

    if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
    {
        vector<CGameObject*>* Overlaps = { nullptr };
        if (false == m_pColliderCom->GetOverlapAll(Overlaps))
            return;

        for (auto iter : *Overlaps)
        {
            Check_Collision_Enemy(iter);
            Check_Collision_Portal(iter);
            Check_Collision_Key(iter);
        }

        Check_Collision_ToodeState();
        Check_Collision_PlayerPos();
    }

    Check_Collision_Ground();
}

void CPlayer_Thirdee::Check_Collision_ToodeState()
{
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

void CPlayer_Thirdee::Check_Collision_Ground()
{
    _float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
    vPosition = _float3(vPosition.x, vPosition.y, vPosition.z - m_fGroundCheckColliderY);

    m_pGroundCheckColliderCom->ApplyFixedPosition(vPosition);

    m_pGroundCheckColliderCom->Collision_On();

    m_pGameInstance->Check_Collision(m_pGroundCheckColliderCom);


    if (m_pGroundCheckColliderCom->OnCollisionEnter() || m_pGroundCheckColliderCom->OnCollisionStay())
    {
        vector<CGameObject*>* Overlaps = { nullptr };
        if (false == m_pGroundCheckColliderCom->GetOverlapAll(Overlaps))
            return;

        for (auto iter : *Overlaps)
        {
            if ((iter->Get_Name().find(TEXT("Wall")) != wstring::npos
                || iter->Get_Name().find(TEXT("Block")) != wstring::npos))
            {
                if(false ==m_bJumpStart)
                {
                    m_bInAction = false;
                    m_fCurrentGravity = 0.f;
                    m_fCurrentJumpPower = 0.f;
                    m_bFalling = false;
                }
            }

            if (iter->Get_Name().find(TEXT("Break")) != wstring::npos)
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
    }
    else
    {
        m_bFalling = true;

        if(false == m_bInAction)
        {
            m_bInAction = true;
            m_fCurrentGravity = 0.f;
            m_fCurrentJumpPower = -5.f;
            m_pCurrentState->Request_ChangeState(this, PLAYERSTATE::ACTION);
        }
    }
}

void CPlayer_Thirdee::Toodee_Direction(_uint iInputData, _float fTimeDelta)
{
    if ((iInputData & ENUM_CLASS(KEYINPUT::KEY_MOVES)) != 0)
    {
        _float3 vLook = m_pTransformCom->Get_State(STATE::POSITION);

        _float3 vDir = { 0.f,0.f,0.f };

        if (iInputData & ENUM_CLASS(KEYINPUT::KEY_UP))
        {
            vDir.y += 2.f;
        }
        if (iInputData & ENUM_CLASS(KEYINPUT::KEY_DOWN))
        {
            vDir.y -= 2.f;
        }

        if (D3DXVec3Length(&vDir) <= 0.0001)
            return;

        vLook += vDir;

        m_pTransformCom->Look_At_FixUp(vLook);
    }
}

void CPlayer_Thirdee::Toodee_Move(_float fTimeDelta)
{
    m_pTransformCom->Go_Left(fTimeDelta);
}

void CPlayer_Thirdee::Toodee_Action(_float _fTimeDelta)
{
    m_fAccumulationJumpPower += 2.f;

    if (m_fAccumulationJumpPower > 15.f)
        return;
    
    m_fCurrentJumpPower += 2.f;
}


CPlayer_Thirdee* CPlayer_Thirdee::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayer_Thirdee* pInstance = new CPlayer_Thirdee(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed Created : CPlayer_Thirdee"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayer_Thirdee::Clone(void* pArg)
{
    CPlayer_Thirdee* pInstance = new CPlayer_Thirdee(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed Cloned : CPlayer_Thirdee"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CPlayer_Thirdee::Free()
{
    __super::Free();
    
    for (auto& StatePair : m_States)
        Safe_Release(StatePair.second);
    m_States.clear();

    Safe_Release(m_pCurrentState);
    Safe_Release(m_pRootPart);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pGroundCheckColliderCom);
    Safe_Release(m_pAttachCheckColliderCom);
}
