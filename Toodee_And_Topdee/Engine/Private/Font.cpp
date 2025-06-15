#include "Font.h"

CFont::CFont(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device { pGraphic_Device }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CFont::Initialize(FONT_DESC* pFontDesc)
{
    if (nullptr == pFontDesc)
        return E_FAIL;

    D3DXCreateSprite(m_pGraphic_Device, &m_pSprite);

    D3DXFONT_DESC LogFont = {};
    ZeroMemory(&LogFont, sizeof(D3DXFONT_DESC));
    LogFont.Height = pFontDesc->fHegiht;
    LogFont.Width = pFontDesc->fWidth;
    LogFont.Weight = pFontDesc->fWeight;
    LogFont.Italic = false;
    LogFont.MipLevels = 0;
    LogFont.CharSet = DEFAULT_CHARSET;
    LogFont.OutputPrecision = OUT_DEFAULT_PRECIS;
    LogFont.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;;
    wcscpy_s(LogFont.FaceName, pFontDesc->FontName);

    D3DXCreateFontIndirectW(m_pGraphic_Device, &LogFont, &m_pFont);
    
    return S_OK;
}

void CFont::DrawFont(const _tchar* pDrawText, LPRECT pRect, D3DCOLOR dwColor)
{
    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
    m_pFont->DrawTextW(m_pSprite, pDrawText, -1, pRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE, dwColor);
    m_pSprite->End();
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device, FONT_DESC* pFontDesc)
{
    CFont* pInstance = new CFont(pGraphic_Device);
    if (FAILED(pInstance->Initialize(pFontDesc)))
    {
        MSG_BOX(TEXT("Failed Created : CFont"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFont::Free()
{
    __super::Free();

    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pFont);
    Safe_Release(m_pSprite);
}
