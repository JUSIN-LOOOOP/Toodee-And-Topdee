#include "Thirdee_Hair.h"

CThirdee_Hair::CThirdee_Hair(LPDIRECT3DDEVICE9 pGraphic_Deivce)
	: CThirdee_Part{ pGraphic_Deivce }
{
}

CThirdee_Hair::CThirdee_Hair(const CThirdee_Hair& Prototype)
	: CThirdee_Part{ Prototype }
{
}

HRESULT CThirdee_Hair::Initialize_Prototype()
{

	return E_FAIL;
}

HRESULT CThirdee_Hair::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize_Part(pArg);

	m_fAnimTime = 0.f;
	m_fCurrentAnimRadian = 0.f;
	m_fRadianPerSec = D3DXToRadian(5.f);

	return S_OK;
}

void CThirdee_Hair::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CThirdee_Hair::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CThirdee_Hair::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CThirdee_Hair::Render()
{
	__super::Render();

	return S_OK;
}

void CThirdee_Hair::AnimUpdate(_float fTimeDelta, PLAYERSTATE eState)
{

}

CThirdee_Hair* CThirdee_Hair::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CThirdee_Hair* pInstance = new CThirdee_Hair(pGraphic_Device);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed Created : CThirdee_Hair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThirdee_Hair::Clone(void* pArg)
{
	MSG_BOX(TEXT("CThirdee_Hair Class is Can't Cloned"));

	return nullptr;
}

void CThirdee_Hair::Free()
{
	__super::Free();
}
