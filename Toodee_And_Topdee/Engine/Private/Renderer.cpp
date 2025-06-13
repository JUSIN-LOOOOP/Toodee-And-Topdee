#include "Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device{ pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == pRenderObject)
		return E_FAIL;

	m_RenderObjects[ENUM_CLASS(eRenderGroup)].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_Tile()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_PRIORITY)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_PRIORITY)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Tile()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_TILE)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_TILE)].clear();


	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONBLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONBLEND)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_BLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_BLEND)].clear();

	return S_OK;
}
HRESULT CRenderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_UI)].clear();

	return S_OK;
}

CRenderer* CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return new CRenderer(pGraphic_Device);
}

void CRenderer::Free()
{
	__super::Free();
	for (size_t i = 0; i < ENUM_CLASS(RENDERGROUP::RG_END); i++)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
			Safe_Release(pRenderObject);

		m_RenderObjects[i].clear();
	}

	Safe_Release(m_pGraphic_Device);
}
