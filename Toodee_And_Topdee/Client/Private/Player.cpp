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
#include "State_Swim.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBlendObject { pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CBlendObject{ Prototype }
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

	m_pGameInstance->StopSound(CHANNELID::SOUND_PLAYER);
	m_pGameInstance->PlayAudio(TEXT("Clear.wav"), CHANNELID::SOUND_PLAYER, 0.5f);


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
	case PLAYERSTATE::SWIM:
		pInstance = CState_Swim::Create(pArg);
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

HRESULT CPlayer::Begin_Shader()
{
	CShader_Player::SHADERPLAYER_DESC pDesc{};
	_float4x4 WorldMatrix = *m_pTransformCom->Get_WorldMatrix();
	_float4x4 ViewMatrix;
	_float4x4 ProjMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	pDesc.WorldMatrix = WorldMatrix;
	pDesc.ViewMatrix = ViewMatrix;
	pDesc.ProjMatrix = ProjMatrix;

	m_pShader->Bind_Shader(&pDesc);
	
	return S_OK;
}

HRESULT CPlayer::End_Shader()
{
	m_pShader->Reset_Shader();

	return S_OK;
}


HRESULT CPlayer::Render_Shadow()
{
	if (m_eCurrentState == PLAYERSTATE::STOP) return S_OK;

	Compute_AttributeShadow();

	_float3 vGroundPoint = _float3(0.f, -0.5f, 0.f);
	_float3 vGroundNormal = _float3(0.f, 1.f, 0.f);
	D3DXPLANE groundPlane;
	D3DXPlaneFromPointNormal(&groundPlane, &vGroundPoint, &vGroundNormal);

	_float angle = D3DXToRadian(m_fCurAngle);
	_float3 lightDir = _float3(cosf(angle), m_fCurLightY, sinf(angle));
	D3DXVec3Normalize(&lightDir, &lightDir);
	_float4 light = _float4(lightDir.x, lightDir.y, lightDir.z, 0.0f);

	_float4x4 matShadow;
	D3DXMatrixShadow(&matShadow, &light, &groundPlane);

	_float4x4 matWorld = *m_pTransformCom->Get_WorldMatrix();
	matWorld._11 = 1.f;    matWorld._12 = matWorld._13 = 0.f;
	matWorld._22 = 1.f;     matWorld._21 = matWorld._23 = 0.f;
	matWorld._33 = 1.f;    matWorld._31 = matWorld._32 = 0.f;
	matWorld._43 -= 0.3f;
	_float4x4 matrixShadow = matShadow * matWorld;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &matrixShadow);

	//텍스처 스테이지 상태 백업
	DWORD oldColorOp, oldColorArg1, oldAlphaOp, oldAlphaArg1;
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLOROP, &oldColorOp);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLORARG1, &oldColorArg1);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_ALPHAOP, &oldAlphaOp);
	m_pGraphic_Device->GetTextureStageState(0, D3DTSS_ALPHAARG1, &oldAlphaArg1);

	//렌더 상태 백업
	DWORD oldAlphaEnable, oldSrcBlend, oldDestBlend, oldZWrite, oldLighting;
	m_pGraphic_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &oldAlphaEnable);
	m_pGraphic_Device->GetRenderState(D3DRS_SRCBLEND, &oldSrcBlend);
	m_pGraphic_Device->GetRenderState(D3DRS_DESTBLEND, &oldDestBlend);
	m_pGraphic_Device->GetRenderState(D3DRS_ZWRITEENABLE, &oldZWrite);
	m_pGraphic_Device->GetRenderState(D3DRS_LIGHTING, &oldLighting);

	//스텐실 버퍼
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILREF, 0x0);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);


	//그림자 렌더 설정
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<_uint>(110.f * m_fCurLightY), 8, 8, 8));
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	// 그림자 렌더링
	m_VIBufferCom_Diffuse->Bind_Buffers();
	m_VIBufferCom_Diffuse->Render();

	//원복
	m_pGraphic_Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, oldColorArg1);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, oldAlphaArg1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, oldAlphaEnable);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, oldSrcBlend);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, oldDestBlend);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, oldZWrite);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, oldLighting);
	return S_OK;
}

void CPlayer::Compute_AttributeShadow()
{
	if (!m_bCameraChange && m_pGameInstance->Get_CurrentDimension() == DIMENSION::CHANGE)
	{
		m_bCameraChange = true;
	}

	if (m_bCameraChange && !m_bComputeComplete) {
		if (m_ePreDimension == DIMENSION::TOPDEE)
		{
			if (m_fCurLightY <= m_fToodeeLightDirY)
				m_fCurLightY += 0.05f;

			if (m_fCurAngle >= m_fToodeeAngle - 360.f)
				m_fCurAngle -= 4.f;
			else
			{
				m_fCurLightY = m_fToodeeLightDirY;
				m_fCurAngle = m_fTopdeeAngle;
				m_bComputeComplete = true;
			}
		}
		else
		{
			if (m_fCurLightY <= m_fTopdeeLightDirY)
				m_fCurLightY -= 0.03f;

			if (m_fCurAngle >= m_fTopdeeAngle)
				m_fCurAngle -= 4.f;
			else
			{
				m_fCurLightY = m_fTopdeeLightDirY;
				m_fCurAngle = m_fTopdeeAngle;
				m_bComputeComplete = true;
			}

		}
	}
	if (m_bComputeComplete && m_pGameInstance->Get_CurrentDimension() != DIMENSION::CHANGE)
	{
		m_bComputeComplete = false;
		m_bCameraChange = false;
		m_ePreDimension = m_pGameInstance->Get_CurrentDimension();
		if (m_ePreDimension == DIMENSION::TOODEE)
		{
			m_fCurAngle = m_fToodeeAngle; m_fCurLightY = m_fToodeeLightDirY;
		}
		else
		{
			m_fCurAngle = m_fTopdeeAngle; m_fCurLightY = m_fTopdeeLightDirY;
		}
	}
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_VIBufferCom_Diffuse);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShader);

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
