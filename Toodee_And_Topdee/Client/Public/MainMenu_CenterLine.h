#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CMainMenu_CenterLine final : public CGameObject
{
private:
	CMainMenu_CenterLine(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_CenterLine(const CMainMenu_CenterLine& Prototype);
	virtual ~CMainMenu_CenterLine() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;


private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTransform*				m_pTransformCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };

	_uint					m_iMotionNum = { 0 };
	_float					m_fMotionIntervalTime = { 0.25f };
	_float					m_fAccumulateMotionTime = { 0.f };

private:
	void				Change_Motion(_float fTimeDelta);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CMainMenu_CenterLine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END