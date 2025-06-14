#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
protected:
	CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Begin(class CTransform* pTransform);
	HRESULT End();

	_bool	isPicked(HWND hWnd);

protected:
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float			m_fWinSizeX{}, m_fWinSizeY{};
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};
	_float4x4		m_OldViewMatrix{}, m_OldProjMatrix{};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END