#include "Block.h"
#include "GameInstance.h"

#include "BlockState_Stop.h"
#include "BlockState_Push.h"
#include "BlockState_Attach.h"
#include "BlockState_Detach.h"


CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBlock::CBlock(const CBlock& Prototype)
	: CGameObject{ Prototype }

{
}

HRESULT CBlock::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlock::Initialize(void* pArg)
{
	return S_OK;
}

void CBlock::Priority_Update(_float fTimeDelta)
{
	if (m_bBlock)
		m_bBlock = false;
}

void CBlock::Update(_float fTimeDelta)
{

}

void CBlock::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBlock::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(m_TextureIdx)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();

	return S_OK;
}

HRESULT CBlock::Change_State(BLOCKSTATE eNewState)
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

void CBlock::Push(MOVEDIRECTION eMoveDir, TEXTUREDIRECTION eTextureDir, _float fPushPower)
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

_bool CBlock::Pushed(_float fTimeDelta)
{
	if (m_pTransformCom->Approach(m_vMoveTargetPosition, fTimeDelta, m_fOwnerPushPower))
	{
		m_bMoving = false;
	}

	return !m_bMoving;
}

void CBlock::Attach(CTransform* pPlayerTransform, _float fSpeed)
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


void CBlock::Attached(_float fTimeDelta)
{	
	if(!m_bAttach)
		m_bAttach =m_pTransformCom->Approach(m_vTargetPosition, fTimeDelta, m_fMoveSpeed);
}

void CBlock::Detach(const _float3& vPosition, _float fSpeed)
{
	Safe_Release(m_pOwnerTransform);
	m_pOwnerTransform = nullptr;

	m_vTargetPosition = vPosition;
	m_fMoveSpeed = fSpeed;
}

_bool CBlock::Detached(_float fTimeDelta)
{
	if (m_pTransformCom->Approach(m_vTargetPosition, fTimeDelta, m_fMoveSpeed))
	{
		m_pColliderCom->Collision_On();
		return true;
	}

	return false;
}

void CBlock::FallIntoHole(_float fTimeDelta)
{
}

void CBlock::Update_Attached_Position()
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

_float CBlock::ComputeDirDotLook(const _float3& vPlayerPosition, const _float3& vLook)
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float3 vDirection = vPosition - vPlayerPosition;

	_float3 vNormalLook = {};
	
	D3DXVec3Normalize(&vNormalLook, &vLook);

	D3DXVec3Normalize(&vDirection, &vDirection);

	return D3DXVec3Dot(&vDirection, &vLook);
}

void CBlock::Update_PushState(CBlock* pBlock)
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

CBlockState* CBlock::Find_State(BLOCKSTATE eKeyState)
{
	auto iter = m_States.find(eKeyState);

	if (iter == m_States.end())
		return nullptr;

	return iter->second;
}

HRESULT CBlock::Add_State(BLOCKSTATE eKeyState)
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

void CBlock::Clear_StateParameter()
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

void CBlock::Request_Change_State(BLOCKSTATE eState)
{
	m_pCurrentState->Request_ChangeState(this, eState);
}

void CBlock::Update_State(_float fTimeDelta)
{
	m_pCurrentState->Update(this, fTimeDelta);
}

void CBlock::CheckCollisionState()
{
	if (m_pColliderCom->OnCollisionStay() || m_pColliderCom->OnCollisionEnter())
	{
		if (m_eCurrentState == BLOCKSTATE::PUSH)
		{
			vector<CGameObject*>* pObjects = { nullptr };
			m_pColliderCom->GetOverlapAll(pObjects);

			if(pObjects)
			{
				for (auto iter : *pObjects)
				{
					if (iter->Get_Name() == TEXT("Block_Wall"))
					{
						m_pTransformCom->Set_State(STATE::POSITION, m_vPrevPosition);
						m_pCurrentState->Request_ChangeState(this, BLOCKSTATE::STOP);
						m_bBlock = true;
					}
					else if (iter->Get_Name() == TEXT("Block_Wood"))
					{
						CBlock* pBlock = dynamic_cast<CBlock*>(iter);

						if (false == pBlock->IsPush() && false == pBlock->IsBlock())
						{
							m_pPushBlock = pBlock;
							pBlock->Request_Change_State(BLOCKSTATE::PUSH);
							pBlock->Push(m_eOwnerMoveDir, m_eOnwerTextureDir, m_fOwnerPushPower);
						}
						else if (pBlock->IsBlock())
						{
							m_pTransformCom->Set_State(STATE::POSITION, m_vPrevPosition);
							m_pCurrentState->Request_ChangeState(this, BLOCKSTATE::STOP);
							m_bBlock = true;
						}
					}
				}
			}
		}

	}
}

_bool CBlock::IsOnHole()
{
	return _bool();
}

HRESULT CBlock::Ready_Components()
{
	return S_OK;
}

HRESULT CBlock::Ready_State()
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

void CBlock::SetUp_BlockInfo(void* pArg)
{
	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	m_pTransformCom->Scaling(pDesc->vScale.x, pDesc->vScale.y, 2);
	m_TextureIdx = pDesc->iTextureIdx;
	m_pTransformCom->TurnToRadian(_float3(0.f, 1.f, 0.f), D3DXToRadian(90 * pDesc->iDir));
}

void CBlock::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CBlock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CGameObject* CBlock::Clone(void* pArg)
{
	return nullptr;
}

void CBlock::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	for (auto& StatePair : m_States)
		Safe_Release(StatePair.second);
	m_States.clear();

	Safe_Release(m_pCurrentState);
	Safe_Release(m_pOwnerTransform);
}
