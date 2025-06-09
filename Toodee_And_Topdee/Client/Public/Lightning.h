#pragma once
#include "Client_Defines.h"
#include "PoolableObject.h"


BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;

END

BEGIN(Client)

/* 플레이어가 스파크블럭을 들고 있으면 아예 번개의 콜라이더 작동 안하게 꺼놨습니다.*/

class CLightning final : public CPoolableObject
{
public:
	typedef struct Lightining_Position_Info {
		_float3				vStartPosition = { };
		_float3				vCrashPosition = { };
		_bool				bSparkBlock = { false };
	}LIGHTNING;

private:
	CLightning(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLightning(const CLightning& Prototype);
	virtual ~CLightning() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

	HRESULT				Initialize_Pool(void* pArg);

private:
	CVIBuffer_Cube*		m_pVIBufferCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

	_float3				m_vStartPosition = { };
	_float3				m_vCrashPosition = { };

	_uint				m_iNeedTextureCount = { 1 };
	_float				m_fLightningIntervalDistance = { };
	vector<_float3>		m_vecPositionCenters;

	_uint				m_iMotionNum = { 0 };
	_float				m_fMotionIntervalTime = { 1.f };
	_float				m_fAccumulateMotionTime = { 0.f };

	_bool				m_bIsSparkBlock = { false };


private:
	void				Change_Motion(_float fTimeDelta);
	HRESULT				Bind_Lightning();

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

	

public:
	static CLightning* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END