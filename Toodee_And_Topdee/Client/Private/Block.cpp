#include "Block.h"
#include "GameInstance.h"




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
	Render_Shadow();

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

_float CBlock::ComputeDirDotLook(const _float3& vPlayerPosition, const _float3& vLook)
{
	_float3 vPosition = m_pTransformCom->Get_State(STATE::POSITION);

	_float3 vDirection = vPosition - vPlayerPosition;

	vDirection.y = 0.f;

	_float3 vNormalLook = vLook;

	vNormalLook.y = 0.f;

	D3DXVec3Normalize(&vNormalLook, &vNormalLook);

	D3DXVec3Normalize(&vDirection, &vDirection);

	return D3DXVec3Dot(&vDirection, &vNormalLook);
}

HRESULT CBlock::Render_Shadow()
{
	Compute_AttributeShadow();

	_float3 vGroundPoint = _float3(0.f, -0.51f, 0.f);
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
	matWorld._11 = 2.f;    matWorld._12 = matWorld._13 = 0.f;
	matWorld._22 = 1.f;     matWorld._21 = matWorld._23 = 0.f;
	matWorld._33 = 2.f;    matWorld._31 = matWorld._32 = 0.f;
	m_matrixShadow = matShadow * matWorld;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_matrixShadow);

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
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(static_cast<_uint>(110.f* m_fCurLightY), 8, 8, 8));
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

void CBlock::Compute_AttributeShadow()
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
			if (m_fCurLightY <= m_fTopdeeLightDirY )
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
	if (m_bComputeComplete&& m_pGameInstance->Get_CurrentDimension() != DIMENSION::CHANGE)
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

HRESULT CBlock::Ready_Components()
{
	return S_OK;
}


void CBlock::SetUp_BlockInfo(void* pArg)
{
	BLOCK_INFO* pDesc = static_cast<BLOCK_INFO*>(pArg);
	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vPos);
	m_pTransformCom->Scaling(pDesc->vScale.x, pDesc->vScale.y, 2);
	m_pTransformCom->Go_Up(0.2f);
	m_TextureIdx = pDesc->iTextureIdx;
	m_pTransformCom->TurnToRadian(_float3(0.f, 1.f, 0.f), D3DXToRadian(90 * pDesc->iDir));

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

void CBlock::SetUp_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CBlock::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBlock::Free()
{
	__super::Free();

	Safe_Release(m_VIBufferCom_Diffuse);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
