#include "Player.h"
#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject { Prototype }
	, m_fAnimDelay{ Prototype.m_fAnimDelay }
	, m_ePreDir { Prototype.m_ePreDir }
	, m_eCurrentDir { Prototype.m_eCurrentDir }
	, m_fPotalDistance { Prototype.m_fPotalDistance }
{
	for (_uint i = 0; i < ENUM_CLASS(PS_DEAD); i++)
	{
		m_iMaxAnimCount[i] = Prototype.m_iMaxAnimCount[i];
	}

}

void CPlayer::Change_State()
{
	if (m_ePreState == m_eCurrentState)
		return;

	m_ePreState = m_eCurrentState;
	m_iCurrentAnimCount = 0;
}

void CPlayer::Change_Dir()
{
	if (m_ePreDir == m_eCurrentDir)
		return;

	m_ePreDir = m_eCurrentDir;

	m_pTransformCom->TurnToRadian(_float3(0.f, 0.f, 1.f), D3DXToRadian(180.f));

}

_bool CPlayer::MoveToPotal(const _float3& vTarget, const _float3& vAxis, _float fTimeDelta)
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);
	

	if(m_eCurrentState != PS_CLEAR )
	{
		if (m_fClearSpeedPerSec == 0.f)
		{
			m_vPotalStartPosition = { vTarget.x + m_fPotalDistance, vTarget.y, vTarget.z };

			_float3 vSpeed = m_vPotalStartPosition - vPosition;

			m_fClearSpeedPerSec = D3DXVec3Length(&vSpeed);

		}

		if(m_pTransformCom->Move_To(m_vPotalStartPosition, fTimeDelta, m_fClearSpeedPerSec, 0.f))
		{
			m_eCurrentState = PS_CLEAR;
			m_fClearSpeedPerSec = 0.f;
		}

	}
	else if (m_eCurrentState == PS_CLEAR)
	{
		_float fDegree = 480.f;

		if (m_pTransformCom->Spiral(vTarget, vAxis, fDegree, m_fPotalDistance, fTimeDelta))
		{
			m_bClear = true;
			return true;
		}

	}

	return false;
}
void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	for (_uint i = 0; i < static_cast<_uint>(PS_DEAD); i++)
	{
		Safe_Release(m_pTextureComs[i]);
	}
}
