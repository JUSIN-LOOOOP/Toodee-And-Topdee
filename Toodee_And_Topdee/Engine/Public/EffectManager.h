#pragma once
#include "Base.h"

BEGIN(Engine)

class CEffectManager final :  public CBase
{
private:
	CEffectManager();
	virtual ~CEffectManager() = default;

public:
	void	Update(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT				Add_PSystem(class CPSystem* pPSystem, const _wstring& strEffectTag);
	HRESULT				Set_Active(const _wstring& strEffectTag, void *arg = nullptr);

private:
	map<const _wstring, class CPSystem*> m_PSystems;

	class CGameInstance* m_pGameInstance = { nullptr };

private :
	class CPSystem* Find_Pools(const _wstring strEffectTag);

public:
	static CEffectManager* Create();
	virtual void Free() override;
};

END