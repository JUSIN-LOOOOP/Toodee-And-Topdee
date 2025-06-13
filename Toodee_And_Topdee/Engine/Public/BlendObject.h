#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CBlendObject abstract : public CGameObject
{
protected:
	CBlendObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlendObject(const CBlendObject& Prototype);
	virtual ~CBlendObject() = default;

public:
	_float Get_Depth() const {
		return m_fCamDistance;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float			m_fCamDistance = {};

protected:
	void Compute_CamDistance(class CTransform* pTransformCom);
	void BillBoard(class CTransform* pTransformCom);
	void BillBoard_XZ(class CTransform* pTransformCom);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END