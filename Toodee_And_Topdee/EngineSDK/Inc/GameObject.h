#pragma once
#include "Base.h"

/* 게임오브젝트들의 부모가 되는 클래스 */

/*	주요 기능
			사용자가 정의한 라이프사이클를 가상 함수로 추상화함.
			게임 오브젝트 객체에 붙일 사본 컴포넌트를 생성 및 보관
*/

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	class CComponent* Get_Component(const _wstring& strComponentTag);

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	map<const _wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag,
		const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

	_wstring name = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END