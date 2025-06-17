#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
	ZeroMemory(m_bPrevKeyState, sizeof(m_bPrevKeyState));
}

void CKey_Manager::Update()
{
	for (_int iKey : keysToCheck) 
	{	
		m_bPrevKeyState[iKey] = m_bKeyState[iKey];  // ���� Ű ���� ����
		m_bKeyState[iKey] = (GetAsyncKeyState(iKey) & 0x8000) ? true : false;  // ���� Ű ���� ����
	}

}

_bool CKey_Manager::Key_Pressing(int _iKey)
{
	return m_bKeyState[_iKey];
}

_bool CKey_Manager::Key_Up(int _iKey)
{
	return m_bPrevKeyState[_iKey] && !m_bKeyState[_iKey];
}

_bool CKey_Manager::Key_Down(int _iKey)
{
	return !m_bPrevKeyState[_iKey] && m_bKeyState[_iKey];
}

HRESULT CKey_Manager::Initialize()
{
	/* ����ϴ� Ű ���� �Է� �޾Ƽ� Update�� ������ ��ȸ */

	keysToCheck.push_back(VK_UP);
	keysToCheck.push_back(VK_DOWN);
	keysToCheck.push_back(VK_LEFT);
	keysToCheck.push_back(VK_RIGHT);
	keysToCheck.push_back(VK_ESCAPE);
	keysToCheck.push_back(VK_RETURN);
	keysToCheck.push_back(VK_SPACE);
	keysToCheck.push_back(static_cast<int>('Z'));
	keysToCheck.push_back(static_cast<int>('X'));
	keysToCheck.push_back(static_cast<int>('R'));

	return S_OK;
}

CKey_Manager* CKey_Manager::Create()
{
	CKey_Manager* pInstance = new CKey_Manager;
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Create : CKey_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CKey_Manager::Free()
{
	__super::Free();
}
