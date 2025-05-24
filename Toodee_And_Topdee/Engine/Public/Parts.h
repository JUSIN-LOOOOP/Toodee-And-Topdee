#pragma once

#include "Component.h"

// 각종 오브젝트들의 파츠(부위)의 부모클래스

BEGIN(Engine)

class ENGINE_DLL CParts abstract : public CComponent
{
public:
	typedef struct tagPart_Desc
	{
		class CGameObject*		pParentObject;
		_wstring				strTexTag;
		_uint					iTexLevelIndex;
		PARTSTATE				eState;
	}PART_DESC;

protected:
	CParts(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParts(const CParts& Prototype);
	virtual ~CParts() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update();
	virtual void	Update( class CTransform* pTransform, _float fTheta, _float fPhi);
	virtual void	Late_Update();
	virtual HRESULT Render();

protected:
	class CGameObject*		m_pParentObject = { nullptr };
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };

	PARTSTATE	m_eState = {};
	_wstring	m_strTexTag = {};
	_uint		m_iTexLevelIndex = {};


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END