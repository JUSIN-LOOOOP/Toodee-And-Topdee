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
private :
	enum MAP_MODE {EDIT, NONEDIT};

private:
	CMap_Manager();
	virtual ~CMap_Manager() = default;

public:
	HRESULT					Initialize();
	void					Update(_float fTimeDelta);

	HRESULT					Load_File(const _wstring& filename);
	HRESULT					Save_File(const _wstring& filename);

	BLOCK_INFO				Get_CurrentType();											//Tile에 정보 입력
	_uint					Get_RenderTextureIdx() { return m_iCurTexture_accum; };		//Tile에 렌더 정보 입력

	void					Add_Tiles(class CGameObject* tile) { m_pTiles.push_back(tile); };	//Save할 때 Tile들로부터 정보 받아올 수 있도록
	HRESULT					Get_Tile_Data(_int idx, BLOCK_INFO& block_data);					//Load할 때 Block이 정보를 받아감
	HRESULT					Load_Initial_Data(vector<_uint>* blockData);						//Editing 모드일 때 블럭 개수, 텍스쳐 개수 넣어줌

private:
	vector<CGameObject*>	m_pTiles;		//Edit 모드시 Tile 관리용 (추후에 스테이지에서 맵 객체 따로 관리할 때도 쓰게 되려나 고민 중)
	vector<BLOCK_INFO>		m_vTiles_data;	//Load 시 읽어온 데이터 저장용
	vector<_uint>			m_vTexture_Counts;
	class CGameInstance* m_pGameInstance = { nullptr };

	_uint					m_iNumTypes = { };
	_uint					m_iNumTextures = { };

	_uint					m_iCurType = { };			//Block데이터
	_uint					m_iCurTexture = { };		//Texture데이터
	_uint					m_iCurDir = {};				//방향데이터

	_uint					m_iCurTexture_accum = {};	//텍스터 데이터 누적. <- Tile, UI 위함
	_uint					m_bOldKey[3] = {};

	MAP_MODE				m_mode = { NONEDIT };

public:
	static CMap_Manager* Create();
	virtual void Free() override;
};

END