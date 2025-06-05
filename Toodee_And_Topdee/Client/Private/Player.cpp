#include "Player.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "PlayerState.h"
#include "State_Idle.h"
#include "State_Move.h"
#include "State_Action.h"
#include "State_Clear.h"
#include "State_Stop.h"
#include "State_Dead.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject { Prototype }
	, m_fPotalDistance { Prototype.m_fPotalDistance }
	, m_bMoveInAction { Prototype.m_bMoveInAction }
	, m_eCurrentTextureDir { Prototype.m_eCurrentTextureDir }
	, m_eActivateDimension { Prototype.m_eActivateDimension }
{
	for (_uint i = 0; i < ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END); i++)
	{
		m_tStateInitDesc[i] = Prototype.m_tStateInitDesc[i];
	}
}

void CPlayer::ClearReady(const CANCLEAREVENT& Event)
{
	Clear(Event.vPosition);

	m_bCanClear = true;
}

HRESULT CPlayer::Change_State(PLAYERSTATE eNewState)
{
	if (m_eCurrentState == eNewState)
		return S_OK;

	if (m_pCurrentState)
	{
		if (eNewState == PLAYERSTATE::STOP)
		{
			m_pPrevState = m_pCurrentState;
			m_iStopAnimCount = ComputeStopAnimCount(m_eCurrentState);
			m_ePrevState = m_pCurrentState->GetTextureKey();
			Safe_AddRef(m_pPrevState);
		}
		else
		{
			m_pCurrentState->Exit(this);
		}
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

void CPlayer::Change_TextureDir(TEXTUREDIRECTION eTextureDirection)
{
	if (m_eCurrentTextureDir == eTextureDirection)
		return;

	m_eCurrentTextureDir = eTextureDirection;

	m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));
}

CPlayerState* CPlayer::Find_State(PLAYERSTATE eKeyState)
{
	auto iter = m_States.find(eKeyState);

	if (iter == m_States.end())
		return nullptr;

	return iter->second;
}
HRESULT CPlayer::Add_State(PLAYERSTATE eKeyState, void* pArg)
{
	if (Find_State(eKeyState) != nullptr)
		return E_FAIL;

	CPlayerState* pInstance = nullptr;

	switch (eKeyState)
	{
	case PLAYERSTATE::IDLE:
		pInstance = CState_Idle::Create(pArg);
		break;
	case PLAYERSTATE::MOVE:
		pInstance = CState_Move::Create(pArg);
		break;
	case PLAYERSTATE::ACTION:
		pInstance = CState_Action::Create(pArg);
		break;
	case PLAYERSTATE::STOP:
		pInstance = CState_Stop::Create(pArg);
		break;
	case PLAYERSTATE::CLEAR:
		pInstance = CState_Clear::Create(pArg);
		break;
	case PLAYERSTATE::DEAD:
		pInstance = CState_Dead::Create(pArg);
		break;
			
	}

	m_States.emplace(eKeyState, pInstance);

	return S_OK;
}
void CPlayer::ComputeTextureDirection(_uint iInputData)
{
	if (iInputData == 0 ||
		(iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT) && iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT)))
		return;

	if (iInputData & ENUM_CLASS(KEYINPUT::KEY_LEFT))
		Change_TextureDir(TEXTUREDIRECTION::LEFT);

	if (iInputData & ENUM_CLASS(KEYINPUT::KEY_RIGHT))
		Change_TextureDir(TEXTUREDIRECTION::RIGHT);

}
_uint CPlayer::ComputeStopAnimCount(PLAYERSTATE eCurrentState)
{
	if (ENUM_CLASS(eCurrentState) >= ENUM_CLASS(PLAYERSTATE::STOP))
		return 0;

	_uint iStateCount = ENUM_CLASS(eCurrentState);
	_uint iTotal = 0;

	while (iStateCount > 0)
	{
		iTotal += m_tStateInitDesc[--iStateCount].iMaxAnimCount;
	}

	iTotal += m_iCurrentAnimCount;

	return iTotal;
}
void CPlayer::Check_Dimension()
{
	if (m_eActivateDimension == m_pGameInstance->Get_CurrentDimension())
	{
		m_pColliderCom->Collision_On();
		m_bCanActive = true;
	}
	else
	{
		m_pColliderCom->Collision_Off();
		m_bCanActive = false;

	}
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);

	for (_uint i = 0; i < ENUM_CLASS(PLAYERSTATE::PLAYERSTATE_END); i++)
	{
		Safe_Release(m_pTextureComs[i]);
	}

	for (auto& StatePair : m_States)
		Safe_Release(StatePair.second);
	m_States.clear();

	Safe_Release(m_pCurrentState);
	Safe_Release(m_pPrevState);

}
