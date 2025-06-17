#include "PSystem.h"
#include "Util.h"
#include "GameInstance.h"

CPSystem::CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device),
	m_pGameInstance{ CGameInstance::GetInstance() }
{
}

CPSystem::CPSystem(const CPSystem& Prototype)
	: CBase( Prototype )
	, m_pVB{ Prototype.m_pVB }
	, m_BoundingBox{ Prototype.m_BoundingBox }
	, m_fSize{ Prototype.m_fSize }
	, m_Particles {Prototype.m_Particles}
{
	Safe_AddRef(m_pVB);
	//Safe_AddRef(m_Texture); ??
}

HRESULT CPSystem::Initialize()
{
	D3DXMatrixIdentity(&worldMatrix);
	m_iNumVertices = 6;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVertexStride * m_iNumVertices * m_Particles.size(), 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return S_OK;
}

void CPSystem::Reset(void* pArg)
{
	_uint count = 0;

	for (auto& particle : m_Particles)
	{
		if (particle._isAlive == false)
		{
			ResetParticle(&particle, pArg);
			++count;
			if (count == m_iChunk)
				return;
		}
	}
}

void CPSystem::Stop(void* pArg)
{
	for (auto& Particle : m_Particles)
	{
		Particle._isAlive = false;
	}
}

void CPSystem::AddParticle()
{
	PARTICLE attribute;

	ResetParticle(&attribute);
	m_Particles.push_back(attribute);
}

void CPSystem::Update(float timeDelta)
{
}

void CPSystem::PreRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

}

HRESULT CPSystem::Render()
{
	if (m_Particles.empty())
		return S_OK;

	PreRender();

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &worldMatrix);
	m_pGraphic_Device->SetTexture(0, m_Texture);
	m_pGraphic_Device->SetFVF(m_iFVF);
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXPOSTEX));

	D3DXMATRIX viewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMatrix);
	D3DXVECTOR3 right(viewMatrix._11, viewMatrix._12, viewMatrix._13);
	D3DXVECTOR3 up(viewMatrix._21, viewMatrix._22, viewMatrix._23);

 
	
	VTXPOSTEX* v = nullptr;

	if (FAILED(m_pVB->Lock(0, 0, (void**)&v, D3DLOCK_DISCARD)))
		return E_FAIL;

	_int count = 0;
	_float halfSizeW = m_fSize * 0.5f;
	_float hlafSizeH = halfSizeW * m_fSizeRatio;

	for (const auto& particle : m_Particles)
	{
		if (particle._isAlive == false)
			continue;

		const _float3& pos = particle._position;
		
		_float3 r = right * halfSizeW * particle._size;
		_float3 u = up * hlafSizeH * particle._size;

		_float frameWidth = 1.0f / static_cast<_float>(m_iNumTextures);
		_float uStart = particle._TextureIdx * frameWidth;
		_float uEnd = uStart + frameWidth;

		v[0] = { pos - r + u, _float2(uStart, 1.f) };
		v[1] = { pos + r + u, _float2(uEnd, 1.f) };
		v[2] = { pos + r - u, _float2(uEnd, 0.f) };
		v[3] = v[0];
		v[4] = v[2];
		v[5] = { pos - r - u, _float2(uStart, 0.f) };

		v += 6;
		++count;
	}

	m_pVB->Unlock();

	if (count > 0)
		m_pGraphic_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, count * 2);

	postRender();

	return S_OK;

}

void CPSystem::postRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

HRESULT CPSystem::isEmpty()
{
	return (m_Particles.size() == 0) ? S_OK : E_FAIL;
}

HRESULT CPSystem::isDead()
{
	for (auto& Particle : m_Particles)
		if (Particle._isAlive == true)
			return E_FAIL;
	return S_OK;
}

void CPSystem::removeDeadParticles()
{
	list<PARTICLE>::iterator i;

	i = m_Particles.begin();

	while (i != m_Particles.end())
	{
		if (i->_isAlive == false)
			i = m_Particles.erase(i);
		else
			i++;
	}
}

void CPSystem::Free()
{
	__super::Free();

	//Safe_Release(m_Texture);
	Safe_Release(m_pVB);
}