#pragma once
#include "Base.h"
#include "GameObject.h"

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

	BLOCK_INFO				Get_CurrentType();													//Tile에 정보 입력

	void					Add_Tiles(class CGameObject* tile) { m_pTiles.push_back(tile); };	//Save할 때 Tile들로부터 정보 받아올 수 있도록
	HRESULT					Get_Tile_Data(_int idx, BLOCK_INFO& block_data);					//Load할 때 Block이 정보를 받아감
	HRESULT					Load_Initial_Data(vector<_uint>* blockData);						//Editing 모드일 때 블럭 개수, 텍스쳐 개수 넣어줌

private:
	vector<CGameObject*>	m_pTiles;					//Edit 모드 시 Tile 관리용 (추후에 스테이지에서 맵 객체 따로 관리할 때도 쓰게 되려나 고민 중)
	vector<BLOCK_INFO>		m_vTiles_data;				//Load 시 읽어온 데이터 저장용
	vector<_uint>			m_vTexture_Counts;			//Edit 모드 시 Type별 Texture가 몇개인 지 정보를 받아옴
	class CGameInstance*	m_pGameInstance = { nullptr };

	_uint					m_iNumTypes = { };			//총 블럭 타입 개수
	_uint					m_iNumTextures = { };		//총 텍스쳐 개수

	BLOCK_INFO				m_tCurInfo = { };
	_uint					m_bOldKey[3] = { };

	MAP_MODE				m_mode = { NONEDIT };

public:
	static CMap_Manager* Create();
	virtual void Free() override;
};

END