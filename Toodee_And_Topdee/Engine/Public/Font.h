#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont final : public CBase
{
public:
	typedef struct tagFontDesc 
	{
		_float fHegiht;
		_float fWidth;
		_float fWeight;
		const _tchar* FontName;
	}FONT_DESC;

private:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CFont() = default;

public:
	HRESULT Initialize(FONT_DESC* pFontDesc);
	void DrawFont(const _tchar* pDrawText, LPRECT pRect, D3DCOLOR dwColor);

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	LPD3DXSPRITE				m_pSprite = { nullptr };
	LPD3DXFONT					m_pFont = { nullptr };

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device, FONT_DESC* pFontDesc);
	virtual void Free() override;
};

END