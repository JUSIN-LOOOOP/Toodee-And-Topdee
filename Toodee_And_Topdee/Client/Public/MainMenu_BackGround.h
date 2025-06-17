#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CMainMenu_BackGround final : public CGameObject
{
private:
	CMainMenu_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_BackGround(const CMainMenu_BackGround& Prototype);
	virtual ~CMainMenu_BackGround() = default;

public:
	HRESULT				Initialize_Prototype() override;
	HRESULT				Initialize(void* pArg) override;
	void				Priority_Update(_float fTimeDelta) override;
	void				Update(_float fTimeDelta) override;
	void				Late_Update(_float fTimeDelta) override;
	HRESULT				Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CMainMenu_BackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END