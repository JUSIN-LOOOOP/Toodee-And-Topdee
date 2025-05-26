#include "Test_Cube.h"
#include "GameInstance.h"


CTest_Cube::CTest_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CTest_Cube::CTest_Cube(const CTest_Cube& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CTest_Cube::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTest_Cube::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


	name = TEXT("1111");
    return S_OK;
}

void CTest_Cube::Priority_Update(_float fTimeDelta)
{
}

void CTest_Cube::Update(_float fTimeDelta)
{

	if (m_pGameInstance->Key_Pressing(VK_UP))
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(VK_DOWN))
		m_pTransformCom->Go_Backward(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(VK_LEFT))
		m_pTransformCom->Go_Left(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(VK_RIGHT))
		m_pTransformCom->Go_Right(fTimeDelta);

	//충돌 진입
	if (m_pColliderCom->OnCollisionEnter())
	{
		m_pTransformCom->Scaling(1.5f, 1.5f, 1.5f);
	}

	//충돌 끝
	if (m_pColliderCom->OnCollisionExit())
	{
		m_pTransformCom->Scaling(1.f, 1.f, 1.f);
	}

	list<CGameObject*>* findAll = { nullptr };

	//충돌 중
	if (m_pColliderCom->OnCollisionStay())
	{
		//이 게임 오브젝트와 충돌중인 모든 오브젝트 들고 오기
		if (m_pColliderCom->GetOverlapAll(findAll))
		{
			for (auto& other : *findAll)
			{
				if (other->CompareName(TEXT("3333")))
				{
					// 코드
				}
			}
		} 

		// 가장 마지막에 들어온 객체 참조하기
		CGameObject* other = m_pColliderCom->GetOverlapTarget();
		_wstring otherName = other->Get_Name();
		
		//코드 

	}



}

void CTest_Cube::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CTest_Cube::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Reset_RenderState();


	return S_OK;
}

HRESULT CTest_Cube::Ready_Components()
{

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_GAMEPLAY), TEXT("Prototype_Component_Texture_TestCube"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;


	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collision"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	{
		MSG_BOX(TEXT("Failed to Add_Component : Com_Collision"));
		return E_FAIL;
	}

	return S_OK;
}

void CTest_Cube::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* 알파 테스트를 수행한다. */
	/* 현재 픽셀을 그릴때 픽셀에 알파값을 비교한다. 통과한 픽셀만 그린다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 현재 그리려고하는 픽셀과 0의 값을 비교한다,ㅡ /.*/
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);

	/* 위에서 비교한 결과, 내가 그릴려고하는 픽셀의 알파가 0보다 크면 통과. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CTest_Cube::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CTest_Cube* CTest_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTest_Cube* pInstance = new CTest_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTest_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTest_Cube::Clone(void* pArg)
{
	CTest_Cube* pInstance = new CTest_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTest_Cube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTest_Cube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);


}
