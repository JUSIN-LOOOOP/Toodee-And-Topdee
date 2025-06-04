#pragma once
#include "Base.h"

BEGIN(Engine)

class CPool_Manager final : public CBase
{
private:
	enum class LEVEL { 
		LEVEL_STATIC, 
		LEVEL_LOADING,
		LEVEL_LOGO,
		LEVEL_GAMEPLAY,
		LEVEL_MAPEDIT,
		LEVEL_END };

private:
	CPool_Manager();
	virtual ~CPool_Manager() = default;

public:
	HRESULT						Initialize(_uint iNumLevels);
	void						Push(_uint iNumLevels, const _wstring& strPoolTag, class CPoolableObject* pGameObject);
	class CPoolableObject*		Pop(_uint iNumLevels, const _wstring& strPoolTag);
	void						Clear(_uint iNumLevels);

private:
	_uint								m_iNumLevels = { 0 };
	map<const _wstring, class CPool*>* m_pPools = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	class CPool* Find_Pools(_uint iNumLevels, const _wstring strPoolTag);

public:
	static CPool_Manager* Create(_uint iNumLevels);
	virtual void Free() override;

};

END