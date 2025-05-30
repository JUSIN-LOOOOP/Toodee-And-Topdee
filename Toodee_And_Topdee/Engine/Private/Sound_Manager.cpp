#include "Sound_Manager.h"
#include <filesystem>


CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
    return S_OK;
}

void CSound_Manager::PlayAudio(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
    map<TCHAR*, FMOD_SOUND*>::iterator iter;

    // iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
    iter = find_if(m_mapSound.begin(), m_mapSound.end(),
        [&](auto& iter)->bool
        {
            return !lstrcmp(pSoundKey, iter.first);
        });

    if (iter == m_mapSound.end())
        return;

    FMOD_BOOL bPlay = FALSE;

    if (FMOD_Channel_IsPlaying(m_pChannelArr[ENUM_CLASS(eID)], &bPlay))
    {
        FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[ENUM_CLASS(eID)]);
    }

    FMOD_Channel_SetVolume(m_pChannelArr[ENUM_CLASS(eID)], fVolume);

    FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
    map<TCHAR*, FMOD_SOUND*>::iterator iter;

    // iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
    iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
        {
            return !lstrcmp(pSoundKey, iter.first);
        });

    if (iter == m_mapSound.end())
        return;

    FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[ENUM_CLASS(CHANNELID::SOUND_BGM)]);
    FMOD_Channel_SetMode(m_pChannelArr[ENUM_CLASS(CHANNELID::SOUND_BGM)], FMOD_LOOP_NORMAL);
    FMOD_Channel_SetVolume(m_pChannelArr[ENUM_CLASS(CHANNELID::SOUND_BGM)], fVolume);
    FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID)
{
    FMOD_Channel_Stop(m_pChannelArr[ENUM_CLASS(eID)]);

}

void CSound_Manager::StopAll()
{
    for (int i = 0; i < ENUM_CLASS(CHANNELID::SOUND_END); ++i)
        FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::SetChannelVolume(CHANNELID eID, float fVolume)
{
    FMOD_Channel_SetVolume(m_pChannelArr[ENUM_CLASS(eID)], fVolume);

    FMOD_System_Update(m_pSystem);
}

void CSound_Manager::LoadSoundFile()
{
    string folderPath = "../Resources/Sounds/";

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        string filePath = entry.path().string();  // 전체 경로
        string fileName = entry.path().filename().string(); // 파일명만

        FMOD_SOUND* pSound = nullptr;
        FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, filePath.c_str(), FMOD_DEFAULT, nullptr, &pSound);

        if (eRes == FMOD_OK)
        {
            int length = static_cast<int>(fileName.length()) + 1;

            TCHAR* pSoundKey = new TCHAR[length];
            ZeroMemory(pSoundKey, sizeof(TCHAR) * length);

            MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, pSoundKey, length);

            m_mapSound.emplace(pSoundKey, pSound);
        }
    }

    FMOD_System_Update(m_pSystem);
}

CSound_Manager* CSound_Manager::Create()
{
	CSound_Manager* pInstance = new CSound_Manager;
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Create : CSound_Manager"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSound_Manager::Free()
{
    __super::Free();

    for (auto& Mypair : m_mapSound)
    {
        delete[] Mypair.first;
        FMOD_Sound_Release(Mypair.second);
    }
    m_mapSound.clear();

    FMOD_System_Release(m_pSystem);
    FMOD_System_Close(m_pSystem);
}
