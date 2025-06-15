#include "FPSCamera.h"
#include "GameInstance.h"


CFPSCamera::CFPSCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CFPSCamera::CFPSCamera(const CFPSCamera& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CFPSCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFPSCamera::Initialize(void* pArg)
{
    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);
    m_pTransformCom->Look_At(pDesc->vAt);

    m_fFovy = pDesc->fFovy;
    m_fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;

    if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOODEE)
        m_bType = CAM_TYPE::TOP;
    if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)
        m_bType = CAM_TYPE::QURTER;

    //CameraTestMoveInitialize();
    m_pGameInstance->Subscribe<CHANGECAM>(m_pGameInstance->Get_NextLevelID(), EVENT_KEY::CHANGE_CAM, [this](const CHANGECAM& Event) {
        this->SetViewFlag(); });

    m_pGameInstance->Subscribe<SHAKING>(m_pGameInstance->Get_NextLevelID(), EVENT_KEY::CAM_SHAKING, [this](const SHAKING& Event) {
        this->SetShaking(Event); });

    m_pPlayer = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(ENUM_CLASS(m_pGameInstance->Get_NextLevelID()), TEXT("Player_TopDee"), TEXT("Com_Transform"), 0));

    LOADINGSCREEN_EVENT event;
    event.bFadeIn = true;
    event.vPos = _float3{ -100.f,-3.f,0.f };
    m_pGameInstance->Publish(ENUM_CLASS(LEVEL::LEVEL_STATIC), EVENT_KEY::LOADINGSCREEN, event);

    return S_OK;
}

void CFPSCamera::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down('X') && m_pGameInstance->Get_CurrentDimension() != DIMENSION::CHANGE) {
        m_pGameInstance->StopSound(CHANNELID::SOUND_RESOURCE);

        if (m_pGameInstance->Get_CurrentDimension() == DIMENSION::TOPDEE)
            m_pGameInstance->PlayAudio(TEXT("CameraDown.wav"), CHANNELID::SOUND_RESOURCE, 0.5f);
        else
            m_pGameInstance->PlayAudio(TEXT("CameraUp.wav"), CHANNELID::SOUND_RESOURCE, 0.5f);

        m_bRotating = true;
        m_pGameInstance->Change_Dimension(DIMENSION::CHANGE);
    }

    if (m_bRotating) {

        ChangeView(fTimeDelta);
    }

    if (GetAsyncKeyState('A'))
    {
        m_pTransformCom->Go_Left(fTimeDelta * 2.f);
    }
    if (GetAsyncKeyState('D'))
    {
        m_pTransformCom->Go_Right(fTimeDelta * 2.f);
    }
    const _float3 PlayerPos = m_pPlayer->Get_State(STATE::POSITION);
    m_pTransformCom->Set_State(STATE::POSITION, { PlayerPos.x + m_offset.x, PlayerPos.y + m_offset.y, PlayerPos.z + m_offset.z });

    if (m_fShaking > 0.f)
        Shaking(fTimeDelta);

    m_ProjMatrix = *(D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));
    m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
}

void CFPSCamera::Update(_float fTimeDelta)
{
    if (GetAsyncKeyState(VK_F7) & 0x8000)
        m_pGameInstance->PlayBGM(L"Test_Loop.mp3", 1.f);



    CameraTestMove(fTimeDelta);
}

void CFPSCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CFPSCamera::Render()
{
    return S_OK;
}

void CFPSCamera::ChangeView(_float fTimeDelta)
{
    _float fDelta = fTimeDelta * 80;
    m_fCurrentAngle += fDelta;

    fDelta = (m_bType == CAM_TYPE::TOP) ? -fDelta : fDelta;
    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::LOOK), D3DXToRadian(fDelta));

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

HRESULT CFPSCamera::Ready_Components(void* pArg)
{

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
        return E_FAIL;

    return S_OK;
}

void CFPSCamera::CameraTestMoveInitialize()
{
    m_OldPoint.x = g_iWinSizeX >> 1;
    m_OldPoint.y = g_iWinSizeY >> 1;

    POINT		ptMouse = m_OldPoint;
    ClientToScreen(g_hWnd, &ptMouse);

    SetCursorPos(ptMouse.x, ptMouse.y);
}

void CFPSCamera::CameraTestMove(_float fTimeDelta)
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

     //POINT			ptMouse{};

     //GetCursorPos(&ptMouse);
     //ScreenToClient(g_hWnd, &ptMouse);

     //_int		iMouseMove = {};

     //if (iMouseMove = ptMouse.x - m_OldPoint.x)
     //{
     //    m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), iMouseMove * fTimeDelta * 0.1f);
     //}

     //if (iMouseMove = ptMouse.y - m_OldPoint.y)
     //{
     //    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), iMouseMove * fTimeDelta * 0.1f);
     //}

     //m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
     //m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFovy, m_fAspect, m_fNear, m_fFar));

     //m_OldPoint = ptMouse;
}

void CFPSCamera::Shaking(_float fTimeDelta)
{
    m_fShaking -= fTimeDelta;

    _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
    m_pTransformCom->Set_State(STATE::POSITION, _float3{ pos.x + m_pGameInstance->Rand(-.2f, .2f),
                                                         pos.y + m_pGameInstance->Rand(-.2f, .2f),
                                                         pos.z + m_pGameInstance->Rand(-.2f, .2f) });
}

CFPSCamera* CFPSCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFPSCamera* pInstance = new CFPSCamera(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CFPSCamera"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFPSCamera::Clone(void* pArg)
{
    CFPSCamera* pInstance = new CFPSCamera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CFPSCamera"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFPSCamera::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
}
