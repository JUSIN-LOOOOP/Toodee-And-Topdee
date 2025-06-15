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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}
