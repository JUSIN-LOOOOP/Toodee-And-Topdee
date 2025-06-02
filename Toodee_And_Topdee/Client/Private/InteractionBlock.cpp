#include "InteractionBlock.h"
#include "GameInstance.h"
#include "BlockState_Stop.h"
#include "BlockState_Push.h"
#include "BlockState_Attach.h"
#include "BlockState_Detach.h"
#include "Hole.h"

CInteractionBlock::CInteractionBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlock{ pGraphic_Device }
{
}

CInteractionBlock::CInteractionBlock(const CInteractionBlock& Prototype)
	: CBlock { Prototype }
{
}

HRESULT CInteractionBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInteractionBlock::Initialize(void* pArg)
{
	return S_OK;
}

void CInteractionBlock::Priority_Update(_float fTimeDelta)
{
	if (m_bBlock)
		m_bBlock = false;
}

void CInteractionBlock::Update(_float fTimeDelta)
{
}

void CInteractionBlock::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CInteractionBlock::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CInteractionBlock::Change_State(BLOCKSTATE eNewState)
{
	if (m_eCurrentState == eNewState)
		return S_OK;

	if (m_pCurrentState)
	{
		m_pCurrentState->Exit(this);

		Safe_Release(m_pCurrentState);
	}

	Clear_StateParameter();

	auto iterState = m_States.find(eNewState);
	if (iterState == m_States.end())
		return E_FAIL;

	m_pCurrentState = iterState->second;
	m_eCurrentState = eNewState;

	m_pCurrentState->Enter(this);
	Safe_AddRef(m_pCurrentState);

	return S_OK;
}

void CInteractionBlock::Request_Change_State(BLOCKSTATE eState)
{
	m_pCurrentState->Request_ChangeState(this, eState);
}

void CInteractionBlock::Push(MOVEDIRECTION eMoveDir, TEXTUREDIRECTION eTextureDir, _float fPushPower)
{
	if (m_bMoving)
		return;

	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	m_vPrevPosition = vPosition;

	_float3 vTargetPosition = {};

	const _float TILE_SIZE = 2.f;

	_float fDistanceX = {};
	_float fDistanceZ = {};

	switch (eMoveDir)
	{
	case MOVEDIRECTION::UP:
		fDistanceZ = TILE_SIZE;
		break;
	case MOVEDIRECTION::DOWN:
		fDistanceZ = -TILE_SIZE;
		break;
	case MOVEDIRECTION::TRANSVERSE:
		fDistanceX = TILE_SIZE;
		break;
	}

	if (eTextureDir == TEXTUREDIRECTION::LEFT)
	{
		fDistanceX *= -1.f;
	}


	vTargetPosition = { vPosition.x + fDistanceX, vPosition.y, vPosition.z + fDistanceZ };

	m_eOwnerMoveDir = eMoveDir;
	m_eOnwerTextureDir = eTextureDir;
	m_fOwnerPushPower = fPushPower;

	m_vMoveTargetPosition = vTargetPosition;

	m_bMoving = true;
}

_bool CInteractionBlock::Pushed(_float fTimeDelta)
{
	if (m_pTransformCom->Approach(m_vMoveTargetPosition, fTimeDelta, m_fOwnerPushPower))
	{
		m_bMoving = false;
	}

	return !m_bMoving;
}

CBlockState* CInteractionBlock::Find_State(BLOCKSTATE eKeyState)
{
	auto iter = m_States.find(eKeyState);

	if (iter == m_States.end())
		return nullptr;

	return iter->second;
}

HRESULT CInteractionBlock::Add_State(BLOCKSTATE eKeyState)
{
	if (Find_State(eKeyState) != nullptr)
		return E_FAIL;

	CBlockState* pInstance = nullptr;

	switch (eKeyState)
	{
	case BLOCKSTATE::STOP:
		pInstance = CBlockState_Stop::Create(BLOCKSTATE::STOP);
		break;
	case BLOCKSTATE::PUSH:
		pInstance = CBlockState_Push::Create(BLOCKSTATE::PUSH);
		break;
	case BLOCKSTATE::ATTACH:
		pInstance = CBlockState_Attach::Create(BLOCKSTATE::ATTACH);
		break;
	case BLOCKSTATE::DETACH:
		pInstance = CBlockState_Detach::Create(BLOCKSTATE::DETACH);
		break;
	}

	m_States.emplace(eKeyState, pInstance);

	return S_OK;
}

void CInteractionBlock::Update_State(_float fTimeDelta)
{
	m_pCurrentState->Update(this, fTimeDelta);
}

void CInteractionBlock::Clear_StateParameter()
{
	m_eOwnerMoveDir = {};
	m_eOnwerTextureDir = {};
	m_fOwnerPushPower = {};
	m_vMoveTargetPosition = {};
	m_vTargetPosition = {};
	m_fMoveSpeed = {};
	m_pPushBlock = nullptr;
	m_bAttach = false;
	m_bMoving = false;
}

void CInteractionBlock::Update_PushState(CInteractionBlock* pBlock)
{
	if (nullptr == pBlock || nullptr == pBlock->m_pPushBlock)
		return;

	Update_PushState(pBlock->m_pPushBlock);

	if (pBlock->m_pPushBlock->IsBlock())
	{
		m_bBlock = true;
		pBlock->Request_Change_State(BLOCKSTATE::STOP);
		pBlock->m_pTransformCom->Set_State(STATE::POSITION, m_vPrevPosition);
	}
}

void CInteractionBlock::CheckCollisionState()
{
	if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
	{
		vector<CGameObject*>* pObjects = { nullptr };
		m_pColliderCom->GetOverlapAll(pObjects);

		if (pObjects)
		{
			for (auto iter : *pObjects)
			{
				if (m_eCurrentState == BLOCKSTATE::PUSH)
				{

					if (iter->Get_Name() == TEXT("Wall"))
					{
						m_pTransformCom->Set_State(STATE::POSITION, m_vPrevPosition);
						m_pCurrentState->Request_ChangeState(this, BLOCKSTATE::STOP);
						m_bBlock = true;
					}
					else if (iter->Get_Name().find(TEXT("Interaction")) != string::npos)
					{
						CInteractionBlock* pBlock = dynamic_cast<CInteractionBlock*>(iter);

						if (false == pBlock->IsPush() && false == pBlock->IsBlock() && false == pBlock->IsFall())
						{
							m_pPushBlock = pBlock;
							pBlock->Request_Change_State(BLOCKSTATE::PUSH);
							pBlock->Push(m_eOwnerMoveDir, m_eOnwerTextureDir, m_fOwnerPushPower);
						}
						else if (pBlock->IsBlock() || pBlock->IsFall())
						{
							m_pTransformCom->Set_State(STATE::POSITION, m_vPrevPosition);
							m_pCurrentState->Request_ChangeState(this, BLOCKSTATE::STOP);
							m_bBlock = true;
						}

					}
				}
				else if(m_eCurrentState == BLOCKSTATE::STOP && false == m_bFalling)
				{
					if (iter->Get_Name().find(TEXT("Hole")) != string::npos)
					{
						CHole* pHole = dynamic_cast<CHole*>(iter);
						pHole->Set_Dead();
						m_fTotalFallHeight = 0.f;
						m_bFalling = true;
					}
				}
			}
		}

	}
}

_bool CInteractionBlock::IsOnHole()
{
	return _bool();
}

HRESULT CInteractionBlock::Ready_State()
{
	for (_uint i = 0; i < ENUM_CLASS(BLOCKSTATE::BLOCKSTATE_END); i++)
	{
		if (FAILED(Add_State(static_cast<BLOCKSTATE>(i))))
			return E_FAIL;
	}

	m_pCurrentState = Find_State(BLOCKSTATE::STOP);

	if (!m_pCurrentState)
		return E_FAIL;

	Safe_AddRef(m_pCurrentState);

	return S_OK;
}

void CInteractionBlock::Attach(CTransform* pPlayerTransform, _float fSpeed)
{
	if (nullptr != m_pOwnerTransform)
		return;

	m_pOwnerTransform = pPlayerTransform;
	Safe_AddRef(m_pOwnerTransform);

	_float3 vOwnerPosition = m_pOwnerTransform->Get_State(STATE::POSITION);

	_float3 vBoxPosition = m_pTransformCom->Get_State(STATE::POSITION);

	vBoxPosition.x = vOwnerPosition.x;
	vBoxPosition.y = vOwnerPosition.y + 3.f;

	vBoxPosition.z = vOwnerPosition.z;

	m_vTargetPosition = vBoxPosition;
	m_fMoveSpeed = fSpeed;

	m_pColliderCom->Collision_Off();
}

void CInteractionBlock::Attached(_float fTimeDelta)
{
	if (!m_bAttach)
		m_bAttach = m_pTransformCom->Approach(m_vTargetPosition, fTimeDelta, m_fMoveSpeed);
}

void CInteractionBlock::Update_Attached_Position()
{
	if (nullptr == m_pOwnerTransform)
		return;

	_float3 vOwnerPosition = m_pOwnerTransform->Get_State(STATE::POSITION);

	_float3 vBoxPosition = m_pTransformCom->Get_State(STATE::POSITION);

	vBoxPosition.x = vOwnerPosition.x;
	vBoxPosition.y = vOwnerPosition.y + 3.f;
	vBoxPosition.z = vOwnerPosition.z;


	m_pTransformCom->Set_State(STATE::POSITION, vBoxPosition);
}

void CInteractionBlock::Detach(const _float3& vPosition, _float fSpeed)
{
	Safe_Release(m_pOwnerTransform);
	m_pOwnerTransform = nullptr;

	m_vTargetPosition = vPosition;
	m_fMoveSpeed = fSpeed;
}

_bool CInteractionBlock::Detached(_float fTimeDelta)
{
	if (m_pTransformCom->Approach(m_vTargetPosition, fTimeDelta, m_fMoveSpeed))
	{
		m_pColliderCom->Collision_On();
		return true;
	}

	return false;
}

void CInteractionBlock::FallIntoHole(_float fTimeDelta)
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	
	_float fGravity = GRAVITY * fTimeDelta;
	
	m_fTotalFallHeight += fGravity;

	if (m_fTotalFallHeight >= m_fMaxFallHeight)
	{
		fGravity = m_fTotalFallHeight - m_fMaxFallHeight;
		m_pColliderCom->Collision_Off();
		m_bFalling = false;
	}

	vPosition.y -= fGravity;

	m_pTransformCom->Set_State(STATE::POSITION, vPosition);
}

void CInteractionBlock::Free()
{
	__super::Free();

	for (auto& StatePair : m_States)
		Safe_Release(StatePair.second);
	m_States.clear();

	Safe_Release(m_pCurrentState);
	Safe_Release(m_pOwnerTransform);
}