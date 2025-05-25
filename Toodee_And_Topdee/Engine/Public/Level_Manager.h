#pragma once
#include "Base.h"

/* 레벨들을 관리 해주는 클래스 */
/* 주요 기능 
		현재 레벨 보관
		현재 할당된 레벨의 Update()와 Render()를 담당
		레벨 교체 
		레벨 교체 or 파괴시 현재 레벨의 자원들 정리
*/

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT		Open_Level(_uint iLevelID, class CLevel* pNewLevel);
	void		Update(_float fTimeDelta);
	HRESULT		Render();

	const _uint		Get_CurrentLevelID() { return m_iCurrentLevelID; }

private:
	class CLevel* m_pCurrentLevel = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	_uint	m_iCurrentLevelID = {};
	
private:
	HRESULT			Clear_Resources();

public:
	static	CLevel_Manager* Create();
	virtual void Free() override;
};

END