#include "Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
    : m_pSDK(nullptr)
    , m_pGraphic_Device(nullptr)
    , m_pSprite(nullptr)
{
}

HRESULT CGraphic_Device::Ready_GraphicDev(HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY, LPDIRECT3DDEVICE9* ppOut)
{
    m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
    
    if(m_pSDK == nullptr)
        return E_FAIL;

    D3DCAPS9        DeviceCaps;
    ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

    //�׷��� ī�� ���� ������ ����
    if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
        return E_FAIL;

    _ulong dwFlag = 0;

    if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

    else
        dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    Set_Parameters(d3dpp, hWnd, eMode, iSizeX, iSizeY);

    if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphic_Device)))
        return E_FAIL;
    
    if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
        return E_FAIL;
    
    *ppOut = m_pGraphic_Device;

    Safe_AddRef(m_pGraphic_Device);

    return S_OK;
}

// �ĸ� ���� 
void CGraphic_Device::Render_Begin(D3DXCOLOR Color)
{
    m_pGraphic_Device->Clear(0,
                         nullptr,
                         D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
                         Color, 	// �� ������ ����
                         1.f,	    // ���� ���� �ʱ�ȭ ��
                         0);		// ���ٽ� ���� �ʱ�ȭ ��
    m_pGraphic_Device->BeginScene();
}

void CGraphic_Device::Render_End(HWND hWnd)
{
    m_pGraphic_Device->EndScene();
    m_pGraphic_Device->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphic_Device::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp, HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY)
{
    d3dpp.BackBufferWidth = iSizeX;
    d3dpp.BackBufferHeight = iSizeY;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;

    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;

    // D3DSWAPEFFECT_DISCARD : ���� ü���� �̿��� ���
    // D3DSWAPEFFECT_FLIP    : ���۸� �ϳ��� �ξ� ��, �޸��� �����ư��� ȭ���� �����ϴ� ���
    // D3DSWAPEFFECT_COPY    : ���� ���۸��� ������ ���

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    d3dpp.hDeviceWindow = hWnd;

    d3dpp.Windowed = static_cast<BOOL>(eMode);

    /* ���� �׽����� ���� ���۸� ��� �����ϰ� �Ұ���������? */
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    // ��üȭ�� ����� �� ����
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

CGraphic_Device* CGraphic_Device::Create(HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY, LPDIRECT3DDEVICE9* ppOut)
{
    CGraphic_Device* pInstance = new CGraphic_Device();

    if (FAILED(pInstance->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppOut)))
    {
        MSG_BOX(TEXT("Failed to Created : CGraphic_Device"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGraphic_Device::Free()
{
    Safe_Release(m_pSprite);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pSDK);
}
