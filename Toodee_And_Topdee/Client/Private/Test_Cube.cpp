#include "Test_Cube.h"
#include "GameInstance.h"
#include <sstream>

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
	_float3 temp = { 0.f,0.f,10.f }; 
	m_pTransformCom->Set_State(STATE::POSITION, temp);

	m_pGameInstance->PlayBGM(L"Test_Loop.mp3", 1.f);

	return S_OK;
}

void CTest_Cube::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Check_Collision(m_pColliderCom);
	
}

void CTest_Cube::Update(_float fTimeDelta)
{	
	if(m_bGravity)
		m_pTransformCom->Go_Backward(fTimeDelta*0.8f);
	
	if (m_pGameInstance->Key_Pressing(VK_UP))
		m_pTransformCom->Go_Straight(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(VK_DOWN))
		m_pTransformCom->Go_Backward(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(VK_LEFT))
		m_pTransformCom->Go_Left(fTimeDelta);

	if (m_pGameInstance->Key_Pressing(VK_RIGHT))
		m_pTransformCom->Go_Right(fTimeDelta);

	if (m_pGameInstance->Key_Down(VK_RETURN)) {
		m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
		m_pGameInstance->PlayAudio(TEXT("Test1.wav"), CHANNELID::SOUND_EFFECT, 1.f);
	}
	if (m_pGameInstance->Key_Down(VK_SPACE)) {
		m_pGameInstance->StopSound(CHANNELID::SOUND_EFFECT);
		m_pGameInstance->PlayAudio(TEXT("Test2.wav"), CHANNELID::SOUND_EFFECT, 1.f);
	}
	if (m_pColliderCom->OnCollisionStay())
	{
		m_Dead = true;
		_float dis = {};
		COLLIDER_DIR state = m_pColliderCom->DetectCollisionDirection(&dis);
		if (state == COLLIDER_DIR::BACK)
		{
			_float3 temp = m_pTransformCom->Get_State(STATE::POSITION);
			temp.z -= dis;
			m_pTransformCom->Set_State(STATE::POSITION, temp);

			//m_bGravity = false; 
			int a = 10;
		}
	}
	if (m_pColliderCom->OnCollisionExit())
	{
		int a = 10;
		//MSG_BOX(TEXT("[ OnCollisionExit ]"));
	}

}

void CTest_Cube::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);


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

	if (FAILED(m_pColliderCom->Render()))
		return E_FAIL;

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
	TransformDesc.fSpeedPerSec = 6.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collision */
	CCollider::COLLIDER_DESC  ColliderDesc{};
	ColliderDesc.pOwner = reinterpret_cast<CGameObject*>(this);
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vColliderScale = _float3(1.0f, 1.0f, 2.0f);
	ColliderDesc.bIsFixed = false;

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
