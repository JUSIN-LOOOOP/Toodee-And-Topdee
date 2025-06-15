#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StageBoss_limb.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
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
	vector<CStageBoss_limb*>	m_vlimbs		= {};
	STAGEMONERSTATE				m_eState		= {};
	VIEWMODE					m_eViewMode		= {};
	_float						m_fIdleTime		= {};
	_uint						m_iPlayLevel	= {};
	CVIBuffer_Rect*				m_pVIBufferCom	= { nullptr };
	CTexture*					m_pTextureCom	= { nullptr };
	_uint						m_iDeadCount = 0;
	_float						m_fNonDamagedTime = 0.f;

private :
	HRESULT						Ready_SubscribeEvent(_uint iPlayerLevel);

private :
	HRESULT						Ready_LimbObject(const _wstring& strLayerTag);
	HRESULT						Ready_Component(const _wstring& strLayerTag);
	HRESULT						Begin_RenderState();
	HRESULT						End_RenderState();
	void						isFinish();
	void						isDamaged();

public:
	static CStageBoss* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END 