#pragma once
#include "Base.h"

/* 화면에 그려질 객체들을 그리는 순서대로 분류하여 보관한 클래스 */

/*	주요기능
			매 프레임마다 Render()함수 불리기 전에 화면에 그려 질 게임 오브젝트들을 RENDERGROUP 따라 보관
			보관한 순서대로 객체들을 랜더콜
*/

BEGIN(Engine)

class CRenderer  final : public CBase
{
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw();

private:
	LPDIRECT3DDEVICE9						m_pGraphic_Device = {};
	list<class CGameObject*>				m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_END)];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();


public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END
