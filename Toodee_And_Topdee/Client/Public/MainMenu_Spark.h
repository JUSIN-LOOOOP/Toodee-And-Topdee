#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CTransform;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CMainMenu_Spark final : public CGameObject
{
public:
	typedef struct MainMenu_Spark_Destription
	{
		_uint		iMotionMaxNum = { 1 };
		_float3		vPosition = {};
	}MAINMENU_SPARK;

private:
	CMainMenu_Spark(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMainMenu_Spark(const CMainMenu_Spark& Prototype);
	virtual ~CMainMenu_Spark() = default;

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

	_float3					m_vPosition = { };
	_uint					m_iAlphaValue = {};
	_uint					m_iMotionNum = { 0 };
	_uint					m_iMotionMaxNum = { 0 };
	_float					m_fMotionIntervalTime = { 0.2f };
	_float					m_fAccumulateMotionTime = { 0.f };

private:
	void				Change_Motion(_float fTimeDelta);

private:
	HRESULT				Ready_Components();
	void				SetUp_RenderState();
	void				Reset_RenderState();

public:
	static CMainMenu_Spark* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END