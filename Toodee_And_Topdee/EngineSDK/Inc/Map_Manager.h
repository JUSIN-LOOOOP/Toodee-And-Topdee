#pragma once
#include "Base.h"
#include "GameObject.h"

/*
	현재는
	  1. Edit 모드 : 현재 선택된 타일 type 관리 + Save 기능
	  2. Game Stage : Load 기능
*/

BEGIN(Engine)

class CMap_Manager final : public CBase
{
private:
	CMap_Manager();
	virtual ~CMap_Manager()	= default;

public :
	HRESULT					Initialize(_uint iNumTypes);
	void					Update(_float fTimeDelta);

	HRESULT					Load_File(const _wstring& filename);
	HRESULT					Save_File(const _wstring& filename);
	_int					Get_CurrentType() { return m_iCurrentType; };
	void					Add_Tiles(class CGameObject* tile) { m_pTiles.push_back(tile); };
	HRESULT					Get_Tile_Data(_int idx, BLOCK_INFO& block_data);

private:	
	vector<CGameObject*>	m_pTiles;		//Edit 모드시 Tile 관리용 (추후에 스테이지에서 맵 객체 따로 관리할 때도 쓰게 되려나 고민 중)
	vector<BLOCK_INFO>		m_vTiles_data;	//Load 시 읽어온 데이터 저장용
	class CGameInstance*	m_pGameInstance = { nullptr };

	_uint					m_iNumTypes = { };
	_uint					m_iCurrentType = { };
	_uint					m_bOldKey = {};

public:
	static CMap_Manager* Create(_uint iNumTypes);
	virtual void Free() override;
};

END