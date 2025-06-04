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

    return S_OK;
}

void CMultiViewCamera::Priority_Update(_float fTimeDelta)
{
    /*_bool newkey = GetKeyState('X') & 0x8000;

    if (!m_bOldKey && newkey)
        m_bRotating = true;

    m_bOldKey = newkey;

    if (m_bRotating == true)
        ChangeView(fTimeDelta);*/

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
    if(GetAsyncKeyState(VK_F5)& 0x8000)
        m_pGameInstance->PlayBGM(L"Test_Loop.mp3", 1.f);
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
    // [카메라 속도 기반 세팅]
    _float fDelta = m_ChangeSpeed * fTimeDelta;             //Angle 움직임 속도 제어
    _float fPosDelta = m_ChangeSpeed * fTimeDelta * 1.3;     //Position 움직임 속도 제어
    m_fCurrentAngle += fDelta;

    // [카메라 회전->이동->zoom 적용]
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

    _float3 tmp = m_pTransformCom->Get_State(STATE::POSITION);
    // [목표 각도 달성하면 Flag 해제]
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
