#include "Map_Manager.h"
#include "Util.h"
#include "GameInstance.h"
#include "Tile.h"

CMap_Manager::CMap_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
}

HRESULT CMap_Manager::Initialize()
{
	return S_OK;
}

void CMap_Manager::Update(_float fTimeDelta)
{
	if (m_mode == NONEDIT)
		return;

	/* 마우스 오른쪽 버튼으로 타일 타입 바꾸기 */
	_bool newkey = GetKeyState(VK_RIGHT) & 0x8000;
	if (!m_bOldKey[0] && newkey)
	{
		++m_tCurInfo.iTileTextureIdx;
		++m_tCurInfo.iTextureIdx;
		if (m_tCurInfo.iTextureIdx >= m_vTexture_Counts[m_tCurInfo.iBlockType])
		{
			m_tCurInfo.iTextureIdx = 0;
			++m_tCurInfo.iBlockType;
		}

		if (m_tCurInfo.iBlockType >= m_iNumTypes)
			m_tCurInfo.iBlockType = 0;

		if (m_tCurInfo.iTileTextureIdx >= m_iNumTextures)
			m_tCurInfo.iTileTextureIdx = 0;
	}
	m_bOldKey[0] = newkey;

	/* 마우스 오른쪽 방향키로 타일 회전 */
	newkey = GetKeyState(VK_RBUTTON) & 0x8000;
	if (m_bOldKey[1] && !newkey)
	{
		++m_tCurInfo.iDir;
		if (m_tCurInfo.iDir == 4)
			m_tCurInfo.iDir = 0;
	}
	m_bOldKey[1] = newkey;

	/* S 누르면 타일 저장 */
	newkey = GetKeyState('S') & 0x8000;
	if (!m_bOldKey[2] && newkey)
		Save_File(TEXT("new"));

	m_bOldKey[2] = newkey;
}

HRESULT CMap_Manager::Load_File(const _wstring& filename)
{
	m_vTiles_data.clear();

	ifstream is{ filename , std::ios::binary };
	if (!is)
		return E_FAIL;

	BLOCK_INFO data;

	/*읽은 byte수가 0일 때 까지 "BLOCK_INFO"만큼 계속 읽어서 데이터를 저정한다.*/
	while (true)
	{
		is.read(reinterpret_cast<char*>(&data), sizeof(data));

		if (is.gcount() == 0)
			break;

		m_vTiles_data.push_back(data);
	}

	is.close();

	return S_OK;
}

HRESULT CMap_Manager::Save_File(const _wstring& filename)
{
	ofstream os{ filename , std::ios::binary };
	if (!os)
		return E_FAIL;

	BLOCK_INFO data;

	/*type이 0인 경우를 제외하고 파일에 적는다.*/
	for (auto block : m_pTiles)
	{
		data = dynamic_cast<CTile*>(block)->Get_BlockInfo();
		os.write(reinterpret_cast<char*>(&data), sizeof(BLOCK_INFO));
	}

	os.close();

	MSG_BOX(TEXT("Saved Data"));

	return S_OK;
}

BLOCK_INFO CMap_Manager::Get_CurrentType()
{
	BLOCK_INFO block_info;

	block_info.iBlockType = m_tCurInfo.iBlockType;
	block_info.iDir = m_tCurInfo.iDir;
	block_info.iTextureIdx = m_tCurInfo.iTextureIdx;
	block_info.iTileTextureIdx = m_tCurInfo.iTileTextureIdx;

	return block_info;
}

HRESULT CMap_Manager::Get_Tile_Data(_int idx, BLOCK_INFO& block_data)
{
	if (idx >= m_vTiles_data.size())
		return E_FAIL;
	
	block_data = m_vTiles_data[idx];
	
	return S_OK;
}

HRESULT CMap_Manager::Load_Initial_Data(vector<_uint>* blockData)
{
	if (nullptr == blockData)
		return E_FAIL;

	m_iNumTypes = (_uint)blockData->size();

	if (0 == m_iNumTypes)
		return E_FAIL;

	for (auto iter : *blockData)
	{
		m_iNumTextures += iter;
		m_vTexture_Counts.push_back(iter);
	}

	m_mode = MAP_MODE::EDIT;

	return S_OK;
}

CMap_Manager* CMap_Manager::Create()
{
	CMap_Manager* pInstance = new CMap_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMap_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CMap_Manager::Free()
{
	__super::Free();

	for (auto& pRenderObject : m_pTiles)
		Safe_Release(pRenderObject);
}
