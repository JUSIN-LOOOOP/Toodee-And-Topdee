#pragma once
#include "Base.h"
#include "GameObject.h"

/*
	�����
	  1. Edit ��� : ���� ���õ� Ÿ�� type ���� + Save ���
	  2. Game Stage : Load ���
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
	vector<CGameObject*>	m_pTiles;		//Edit ���� Tile ������ (���Ŀ� ������������ �� ��ü ���� ������ ���� ���� �Ƿ��� ��� ��)
	vector<BLOCK_INFO>		m_vTiles_data;	//Load �� �о�� ������ �����
	class CGameInstance*	m_pGameInstance = { nullptr };

	_uint					m_iNumTypes = { };
	_uint					m_iCurrentType = { };
	_uint					m_bOldKey = {};

public:
	static CMap_Manager* Create(_uint iNumTypes);
	virtual void Free() override;
};

END