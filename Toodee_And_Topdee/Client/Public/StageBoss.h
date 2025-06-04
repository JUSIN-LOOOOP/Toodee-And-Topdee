#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StageBoss_limb.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CStageBoss : public CGameObject
{
private :
	enum class LIMB	{BODY, LEFT_HAND, RIGHT_HAND, LIMB_END };

private:
	CStageBoss(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStageBoss(const CStageBoss& Prototype);
	virtual ~CStageBoss() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;
	void						SetParts(CStageBoss_limb* Parts){ m_vlimbs.push_back(Parts); };

private :
	vector<CStageBoss_limb*>	m_vlimbs	= {};
	STAGEMONERSTATE				m_eState	= {};
	LIMB						m_eAtkLimb	= { LIMB::LIMB_END };
	_float						m_fIdleTime = {};
	VIEWMODE					m_eViewMode = {};

public:
	static CStageBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END