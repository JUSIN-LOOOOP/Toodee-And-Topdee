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
	
	// Level �� �������� �����մϴ�.
	void Clear(_uint iLevelndex);
	
	//�Ŵ����� Observer�� �߰��մϴ�.
	HRESULT Add_Observer(_uint iObserverLevelndex, const _wstring& strObserverTag, class CObserver* pObserver);
	
	//Subject���� ���ϴ� �������� ã�� ����ϰ� ���������� Subject�� ��Ͻ�ŵ�ϴ�.
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