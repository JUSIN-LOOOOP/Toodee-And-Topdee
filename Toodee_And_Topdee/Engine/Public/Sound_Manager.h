#pragma once
#include "Base.h"

#include "Fmod/inc/fmod.hpp"
#include "Fmod/inc/fmod.h"
#include "Fmod/inc/fmod_common.h"

#pragma comment(lib, "fmod_vc.lib")
#include <filesystem>

namespace fs = std::filesystem;
using namespace FMOD;

BEGIN(Engine)

class CSound_Manager final : public CBase
{
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT		Initialize();

public:
	void		 PlayAudio(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void		 PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void		 StopSound(CHANNELID eID);
	void		 StopAll();
	void		 SetChannelVolume(CHANNELID eID, float fVolume);


private:

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[ENUM_CLASS(CHANNELID::SOUND_END)];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

private:
	void		LoadSoundFile();

public:
	static CSound_Manager* Create();
	virtual void Free() override;


};

END