#include "Cannon.h"
#include "GameInstance.h"
#include "PoolableObject.h"
#include "Fire_Projectile.h"

CCannon::CCannon(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CCannon::CCannon(const CCannon& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CCannon::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CCannon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    name = TEXT("Cannon");

    CANNON_INFO* info = reinterpret_cast<CANNON_INFO*>(pArg);
    m_iCannonDir = ENUM_CLASS(info->eDir);
    m_eType = info->eType;

    m_pTransformCom->Set_State(STATE::POSITION, info->vPosition);
    m_pTransformCom->Rotation(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
    
    _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);

    switch (m_iCannonDir)
    {
    case 0:
        m_pTransformCom->Scaling(1.f, 1.9f, 1.f);
		m_pTransformCom->Set_State(STATE::POSITION, _float3(pos.x + 2.f +  (pos.x > 0.f|| fabsf(pos.x / 30.f) < 0.45f ? 0.45f : -pos.x / 30.f), 1.1f, pos.z + 1.f));
        break;

    case 1:
        m_pTransformCom->Scaling(1.f, 1.9f, 1.f);
       m_pTransformCom->Set_State(STATE::POSITION, _float3(pos.x - (pos.x < 0.f || fabsf(pos.x / 30.f) < 0.45f ? 0.45f : pos.x / 30.f), 1.1f, pos.z + 1.f));
        break;

    case 2:
        m_pTransformCom->Scaling(1.9f, 1.f, 1.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3(pos.x + 1.f, 1.1f, pos.z + 2.3f));
        break;

    case 3:
        m_pTransformCom->Scaling(1.9f, 1.f, 1.f);
        m_pTransformCom->Set_State(STATE::POSITION, _float3(pos.x + 1.f, 1.1f, pos.z - 2.3f));
        break;

    }

    m_vOriginalPosition = m_pTransformCom->Get_State(STATE::POSITION);

    m_iLevel = m_pGameInstance->Get_NextLevelID();
    return S_OK;
}

void CCannon::Priority_Update(_float fTimeDelta)
{
}

void CCannon::Update(_float fTimeDelta)
{
    Shooting(fTimeDelta);
    Motion(fTimeDelta);
    //Effect_Motion(fTimeDelta);
}

void CCannon::Late_Update(_float fTimeDelta)
{

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CCannon::Render()
{
    m_pTransformCom->Bind_Matrix();

    if (FAILED(m_pTextureCom->Bind_Texture(m_iCannonDir)))
        return E_FAIL;

    m_pVIBufferCom->Bind_Buffers();
    SetUp_RenderState();
    m_pVIBufferCom->Render();
    Reset_RenderState();

	//if (m_bEffect) {
 //       _float3     pos = m_pTransformCom->Get_State(STATE::POSITION);

 //       switch (m_iCannonDir)
 //       {
 //       case 0:                 /* Right */
 //           pos.x += 0.5f;
 //           pos.y += 0.1f;
 //           m_pTransformCom->Set_State(STATE::POSITION, pos);
 //           m_pTransformCom->Scaling(5.f, 5.f, 5.f);

 //           break;
 //       case 1:                 /* Left */
 //           pos.x -= 0.5f;
 //           pos.y += 0.1f;
 //           m_pTransformCom->Set_State(STATE::POSITION, pos);
 //           m_pTransformCom->Scaling(5.f, 5.f, 5.f);
 //           break;
 //       }
 //       m_pTransformCom->Bind_Matrix();
	//	if (FAILED(m_pTextureCom_Eff->Bind_Texture(m_iEffNum)))
	//		return E_FAIL;

	//	m_pVIBufferCom->Bind_Buffers();
 //       SetUp_AlphaRenderState();
	//	m_pVIBufferCom->Render();
 //       Reset_AlphaRenderState();
 //   }

    return S_OK;
}

void CCannon::Shooting(_float fTimeDelta)
{
    if (m_fIntervalShooting <= m_fAccumulateShootingTime + fTimeDelta)
    {
        m_fAccumulateShootingTime = 0.f;

        m_bMotion = true;
       // m_bEffect = true;
        CFire_Projectile::Protectile_Info info{};

        info.eDir = static_cast<CANNON_DIRECTION>(m_iCannonDir);
        info.vPosition = m_vOriginalPosition;

        CPoolableObject* pProjectile {nullptr};
        _float3 pos = m_pTransformCom->Get_State(STATE::POSITION);
        // todo �߻�ü ��ô
        switch (m_iCannonDir)
        {
        case 0:                 /* Right */
            if (m_eType == CANNON_TYPE::FIRE)
                 pProjectile =  m_pGameInstance->Pop(m_iLevel,ENUM_CLASS(LEVEL::LEVEL_STATIC) ,TEXT("Layer_Projectile_Fire"));
             if (m_eType == CANNON_TYPE::LASER)
                  pProjectile = m_pGameInstance->Pop(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Laser"));
             pos.x += 1.f;
            break;
        case 1:                 /* Left */
            if (m_eType == CANNON_TYPE::FIRE)
                 pProjectile = m_pGameInstance->Pop(m_iLevel, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Fire"));
             if (m_eType == CANNON_TYPE::LASER)
                  pProjectile = m_pGameInstance->Pop(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Laser"));
             pos.x -= 1.0f;
            break;
        case 2:                 /* Up */
            if (m_eType == CANNON_TYPE::FIRE)
                 pProjectile = m_pGameInstance->Pop(m_iLevel, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Fire"));
            if (m_eType == CANNON_TYPE::LASER)
                pProjectile = m_pGameInstance->Pop(m_iLevel, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Laser"));
            break;
            pos.z += 1.f;
        case 3:                 /* Down */
            if (m_eType == CANNON_TYPE::FIRE)
                pProjectile = m_pGameInstance->Pop(m_iLevel, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Fire"));
            if (m_eType == CANNON_TYPE::LASER)
               pProjectile = m_pGameInstance->Pop(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Projectile_Laser"));
            pos.z -= 1.f;
            break;
        }

        if(pProjectile != nullptr)
            pProjectile->Initialize_Pool(&info);

        m_pGameInstance->Set_Active(TEXT("Effect_CannonDust"), &pos);
    }
    else
        m_fAccumulateShootingTime += fTimeDelta;
}

void CCannon::Motion(_float fTimeDelta)
{
    if (!m_bMotion) return;

    if (m_fIntervalMotion <= m_fAccumulateMotionTime + fTimeDelta)
    {
        m_bMotion = false;
        m_fAccumulateMotionTime = 0.f;
        
        if (m_iCannonDir <= 1)
            m_pTransformCom->Scaling(1.f, 1.9f, 1.f);
        else
            m_pTransformCom->Scaling(1.9f, 1.f, 1.f);

        m_pTransformCom->Set_State(STATE::POSITION, m_vOriginalPosition);
    }
    else
    {
        m_fAccumulateMotionTime += fTimeDelta;

        switch (m_iCannonDir)
        {
        case 0:
            m_pTransformCom->Scaling(1.f + m_fAccumulateMotionTime * 1.4f, 1.9f, 1.f);
            m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vOriginalPosition.x + m_fAccumulateMotionTime * 1.3f, m_vOriginalPosition.y, m_vOriginalPosition.z });
            break;

        case 1:
			m_pTransformCom->Scaling(1.f + m_fAccumulateMotionTime * 1.4f, 1.9f, 1.f);
			m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vOriginalPosition.x - m_fAccumulateMotionTime * 1.3f, m_vOriginalPosition.y, m_vOriginalPosition.z });
			break;

		case 2:
			m_pTransformCom->Scaling(1.9f, 1.f + m_fAccumulateMotionTime * 1.4f, 1.f);
			m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vOriginalPosition.x , m_vOriginalPosition.y, m_vOriginalPosition.z + m_fAccumulateMotionTime * 1.3f });
			break;

		case 3:
			m_pTransformCom->Scaling(1.9f, 1.f + m_fAccumulateMotionTime * 1.4f, 1.f);
			m_pTransformCom->Set_State(STATE::POSITION, _float3{ m_vOriginalPosition.x, m_vOriginalPosition.y , m_vOriginalPosition.z - m_fAccumulateMotionTime * 1.3f });
			break;
        }
    }
}

void CCannon::Effect_Motion(_float fTimeDelta)
{
    if (!m_bEffect) return;

    if (m_fEffIntervalMotion <= m_fEffAccumurateTime + fTimeDelta)
    {
        m_fEffAccumurateTime = 0.f;
        ++m_iEffNum;
        if (m_iEffNum >= 3)
        {
            m_bEffect = false;
            m_iEffNum = 0;
        }

    }
    else
        m_fEffAccumurateTime += fTimeDelta;

}

HRESULT CCannon::Ready_Components()
{
    /* For.Com_VIBuffer*/
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cannon"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.m_pTextureCom_Eff */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Effect_Cannon"),
        TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Eff))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{};
    TransformDesc.fSpeedPerSec = 5.f;
    TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom),&TransformDesc)))
        return E_FAIL;

    return S_OK;
}

void CCannon::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    D3DCOLOR_ARGB(20, 255, 255, 255);  // ���� 128 = 50%

}

void CCannon::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

void CCannon::SetUp_AlphaRenderState()
{
    // ���� ����� ����
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // ���ϰ� ����� ���� �ؽ�ó���� ���� (���İ� ����)
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(128, 255, 255, 255)); 

    // �ؽ�ó ����� ���͸� ���ؼ� ���ϰ� �����
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    // ���� ���� �ؽ�ó�� ���͸� ���ؼ� �ε巴��
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

}

void CCannon::Reset_AlphaRenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

}

CCannon* CCannon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCannon* pInstance = new CCannon(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CCannon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCannon::Clone(void* pArg)
{
    CCannon* pInstance = new CCannon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CCannon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCannon::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTextureCom_Eff);
}
