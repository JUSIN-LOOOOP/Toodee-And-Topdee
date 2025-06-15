#include "FontManager.h"

CFontManager::CFontManager()
{
}

HRESULT CFontManager::Initialize()
{
    return S_OK;
}

HRESULT CFontManager::Add_Font(const _wstring& strFontTag, CFont* pFont)
{
    if (nullptr == pFont ||
        nullptr != Find_Font(strFontTag))
        return E_FAIL;

    m_Fonts.emplace(strFontTag, pFont);

    return S_OK;
}

HRESULT CFontManager::DrawFont(const _wstring& strFontTag, const _tchar* pDrawText, LPRECT pRect, D3DCOLOR dwColor)
{
    CFont* pFont = Find_Font(strFontTag);
    if (nullptr == pFont)
        return E_FAIL;

    pFont->DrawFont(pDrawText, pRect, dwColor);

    return S_OK;
}


CFont* CFontManager::Find_Font(const _wstring& strFontTag)
{
    auto iter = m_Fonts.find(strFontTag);

    if (iter == m_Fonts.end())
        return nullptr;

    return iter->second;
}

CFontManager* CFontManager::Create()
{
    CFontManager* pInstance = new CFontManager();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed Created : CFontManager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFontManager::Free()
{
    __super::Free();

    for (auto& Pair : m_Fonts)
        Safe_Release(Pair.second);
    m_Fonts.clear();
}
