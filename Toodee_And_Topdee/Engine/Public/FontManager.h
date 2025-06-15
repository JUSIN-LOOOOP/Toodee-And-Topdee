#pragma once
#include "Base.h"
#include "Font.h"

BEGIN(Engine)

class CFontManager final : public CBase
{
private:
	CFontManager();
	virtual ~CFontManager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_Font(const _wstring& strFontTag, CFont* pFont);

	HRESULT DrawFont(const _wstring& strFontTag, const _tchar* pDrawText, LPRECT pRect, D3DCOLOR dwColor);

private:
	map<const _wstring, CFont*> m_Fonts;

private:
	CFont* Find_Font(const _wstring& strFontTag);

public:
	static CFontManager* Create();
	virtual void Free() override;
};

END