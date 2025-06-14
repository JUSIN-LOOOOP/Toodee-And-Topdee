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

	BLOCK_INFO				Get_CurrentType();													//Tile�� ���� �Է�

	void					Add_Tiles(class CGameObject* tile) { m_pTiles.push_back(tile); };	//Save�� �� Tile��κ��� ���� �޾ƿ� �� �ֵ���
	HRESULT					Get_Tile_Data(_int idx, BLOCK_INFO& block_data);					//Load�� �� Block�� ������ �޾ư�
	HRESULT					Load_Initial_Data(vector<_uint>* blockData);						//Editing ����� �� �� ����, �ؽ��� ���� �־���

private:
	vector<CGameObject*>	m_pTiles;					//Edit ��� �� Tile ������ (���Ŀ� ������������ �� ��ü ���� ������ ���� ���� �Ƿ��� ��� ��)
	vector<BLOCK_INFO>		m_vTiles_data;				//Load �� �о�� ������ �����
	vector<_uint>			m_vTexture_Counts;			//Edit ��� �� Type�� Texture�� ��� �� ������ �޾ƿ�
	class CGameInstance*	m_pGameInstance = { nullptr };

	_uint					m_iNumTypes = { };			//�� �� Ÿ�� ����
	_uint					m_iNumTextures = { };		//�� �ؽ��� ����

	BLOCK_INFO				m_tCurInfo = { };
	_uint					m_bOldKey[3] = { };

	MAP_MODE				m_mode = { NONEDIT };

public:
	static CMap_Manager* Create();
	virtual void Free() override;
};

END