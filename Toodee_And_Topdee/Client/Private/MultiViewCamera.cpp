#include "MultiViewCamera.h"
#include "GameInstance.h"


CMultiViewCamera::CMultiViewCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CMultiViewCamera::CMultiViewCamera(const CMultiViewCamera& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CMultiViewCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMultiViewCamera::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

    m_pTransformCom->Set_State(STATE::POSITION, pDesc->vEye);
    m_pTransformCom->Look_At(pDesc->vAt);

    m_OffsetLength = pDesc->vEye.y;
    m_fFovy = pDesc->fFovy;
    m_fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;

    if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
        m_bType = CAM_TYPE::TOP;
    if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)
        m_bType = CAM_TYPE::QURTER;

    CameraTestMoveInitialize();
    m_pGameInstance->Subscribe<CHANGECAM>(m_pGameInstance->Get_NextLevelID(), EVENT_KEY::CHANGE_CAM, [this](const CHANGECAM& Event) {
        this->SetViewFlag(); });

    return S_OK;
}

void CMultiViewCamera::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down('X') && m_pGameInstance->Get_CurrentDimension() != DIMENSION::CHANGE) {
        m_bRotating = true;
        m_pGameInstance->Change_Dimension(DIMENSION::CHANGE);
    }

    if (m_bRotating) {
    
        ChangeView(fTimeDelta);
    }

    m_ProjMatrix = *(D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
}

void CMultiViewCamera::Update(_float fTimeDelta)
{
    if(GetAsyncKeyState(VK_F7)& 0x8000)
        m_pGameInstance->PlayBGM(L"Test_Loop.mp3", 1.f);

    CameraTestMove(fTimeDelta);
}

void CMultiViewCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CMultiViewCamera::Render()
{
    return S_OK;
}

void CMultiViewCamera::ChangeView(_float fTimeDelta)
{
    _float fDelta = m_ChangeSpeed * fTimeDelta;
    _float fPosDelta = m_ChangeSpeed * fTimeDelta * 1.3f;
    m_fCurrentAngle += fDelta;

    if (m_bType == CAM_TYPE::TOP)
    {
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), -D3DXToRadian(fDelta));
        m_fCurrentPos -= fPosDelta;
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, m_OffsetLength, m_fCurrentPos));
        m_pTransformCom->Go_Straight(fTimeDelta * 3.f);
    }
    else
    {
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), D3DXToRadian(fDelta));
        m_fCurrentPos += fPosDelta;
        m_pTransformCom->Set_State(STATE::POSITION, _float3(0.f, m_OffsetLength, m_fCurrentPos));
        m_pTransformCom->Go_Straight(fTimeDelta * 3.f);
    }

    if (m_fCurrentAngle >= m_fTargetAngle)
    {
        m_fCurrentAngle = 0;
        m_bRotating = false;
        if (m_bType == CAM_TYPE::TOP) {
            m_bType = CAM_TYPE::QURTER;
            m_pGameInstance->Change_Dimension(DIMENSION::TOPDEE);
        }
        else {
            m_bType = CAM_TYPE::TOP;
            m_pGameInstance->Change_Dimension(DIMENSION::TOODEE);
        }
    }
}

HRESULT CMultiViewCamera::Ready_Components(void* pArg)
{
    /* For.Com_Transform */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
        return E_FAIL;

    return S_OK;
}

void CMultiViewCamera::CameraTestMoveInitialize()
{
    m_OldPoint.x = g_iWinSizeX >> 1;
    m_OldPoint.y = g_iWinSizeY >> 1;

    POINT		ptMouse = m_OldPoint;
    ClientToScreen(g_hWnd, &ptMouse);

    SetCursorPos(ptMouse.x, ptMouse.y);
}

void CMultiViewCamera::CameraTestMove(_float fTimeDelta)
{
    if (GetKeyState('W') < 0)
    {
        m_pTransformCom->Go_Straight(fTimeDelta);
    }

    if (GetKeyState('S') < 0)
    {
        m_pTransformCom->Go_Backward(fTimeDelta);
    }

    if (GetKeyState('A') < 0)
    {
        m_pTransformCom->Go_Left(fTimeDelta);
    }

    if (GetKeyState('D') < 0)
    {
        m_pTransformCom->Go_Right(fTimeDelta);
    }

   /* POINT			ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    _int		iMouseMove = {};

    if (iMouseMove = ptMouse.x - m_OldPoint.x)
    {
        m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), iMouseMove * fTimeDelta * 0.1f);
    }

    if (iMouseMove = ptMouse.y - m_OldPoint.y)
    {
        m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), iMouseMove * fTimeDelta * 0.1f);
    }

    m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));

    m_OldPoint = ptMouse;*/
}

CMultiViewCamera* CMultiViewCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMultiViewCamera* pInstance = new CMultiViewCamera(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CMultiViewCamera"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMultiViewCamera::Clone(void* pArg)
{
    CMultiViewCamera* pInstance = new CMultiViewCamera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CMultiViewCamera"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMultiViewCamera::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
}
