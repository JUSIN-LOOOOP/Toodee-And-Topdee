#pragma once
#include "Client_Defines.h"
#include "InteractionBlock.h"
#include "SubjectObject.h"

BEGIN(Client)

class CBlock_Fall final : public CInteractionBlock, public CSubjectObject
{
private:
	CBlock_Fall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlock_Fall(const CBlock_Fall& Prototype);
	virtual ~CBlock_Fall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void onReport(REPORT eReport, CSubjectObject* pSubject) override;

private:
	CCollider* m_pGroundCheckColliderCom = { nullptr };
	CTransform* m_pGroundCheckTransformCom = { nullptr };

	DIMENSION	m_ePrevDimension = {};

	_float3		m_vCurrentTileCenter = {};
	_bool		m_bFall = {};
	_float		m_fGroundCheckPosZ = {};
	_float		m_fGravityPower = {};
	_float		m_fMaxGravityPower = {};

private:
	void CheckDimension();
	_float3 ComputeTileCenter();
	void CheckCollisionToodeeState();

	void Gravity(_float fTimeDelta);

	HRESULT Ready_Components();
	HRESULT Ready_Observer();
	void	SetUp_RenderState();
	void	Reset_RenderState();

public:
	static CBlock_Fall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END

