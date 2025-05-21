#pragma once
#include "Base.h"

/* 원형으로 추가 될 컴포넌트 클래스 타입들을 미리 Include */
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "Transform.h"
#include "Texture.h"

/* 프로토타입 기능을 담당하는 클래스 */

/* 주요 기능
		원형객체(게임오브젝트, 컴포넌트)를 레벨별로 구분하여 보관
		복제하여 만들어진 사본객체를 리턴
		원형객체 Clear() -> 레벨 교체시 사용
*/
BEGIN(Engine)

class CPrototype_Manager : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);
private:
	_uint											m_iNumLevels = {  };
	map<const _wstring, class CBase*>*				m_pPrototypes = { nullptr };
	typedef map<const _wstring, class CBase*>		PROTOTYPES;


private:
	class CBase* Find_Prototype(_uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag);


public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END