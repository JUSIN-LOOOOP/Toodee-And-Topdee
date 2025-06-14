#pragma once
#include "Base.h"

/* 레벨별로 사본 게임오브젝트를 보관하는 클래스 */

/*	주요기능
			레벨별로 사본 게임오브젝트를 보관
			레벨별로 보관되어 있는 사본 게임오브젝트들의 Priority_Update, Update, Late_Update를 담당.
			사본 게임오브젝트 Clear() -> 레벨 교체시 사용
*/

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

public:
	class CComponent* Get_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Get_BackGameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag);

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	_uint									m_iNumLevels = {};
	map<const _wstring, class CLayer*>* m_pLayers = {};

private:
	CLayer* Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

END