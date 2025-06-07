#pragma once
#include "Client_Defines.h"

#include "GameObject.h"


/*
stage1 clouds Position -> { 25.f, 1.9f, 9.f };
*/

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CCloud final : public CGameObject
{
public:
	enum class CLOUD_TYPES { WHITE, BLACK };
	typedef struct tagClouds_Description
	{
		_float3 vPosition;
		CLOUD_TYPES eType;
	}CLOUD_DESC;

private:
	CCloud(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCloud(const CCloud& Prototype);
	virtual ~CCloud() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

private:
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	CLOUD_TYPES			m_eMyType = { CLOUD_TYPES::WHITE };
	_wstring			m_strTexture = { TEXT("Prototype_Component_Texture_Cloud") };
	_uint				m_iMotionNumber = {};
	_float				m_fIntervalMotion = {};
	_float				m_fAccumulateMotion = {};

	_float3				m_ToodeePosition = {};
	_float3				m_TopdeePosition = {};

	DIMENSION			m_eDimension = { DIMENSION::NONE };
	_bool				m_bIsChangeCamera = { false };
	_bool				m_bIsChangePosition = { false };


private:
	void				Motion(_float fTimeDelta);
	void				PositionChangeForCameraSwitch(_float fTimeDelta);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CCloud* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END