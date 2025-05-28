#pragma once
#include "Base.h"

BEGIN(Engine)



class CObserver_Manager final : public CBase
{
private:
	CObserver_Manager();
	virtual ~CObserver_Manager() = default;


public:
	HRESULT Initialize(_uint iNumLevels);
	
	// Level 의 옵저버를 정리합니다.
	void Clear(_uint iLevelndex);
	
	//매니저에 Observer를 추가합니다.
	HRESULT Add_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, class CObserver* pObserver);
	
	//Subject에서 원하는 옵저버를 찾아 등록하고 옵저버에도 Subject를 등록시킵니다.
	HRESULT Subscribe_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, class CSubjectObject* pSubject);

private:
	_uint										m_iNumLevels = {};
	map<const _wstring, class CObserver*>*		m_Observers = {nullptr};

private:
	class CObserver* Find_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag);

public:
	static CObserver_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END