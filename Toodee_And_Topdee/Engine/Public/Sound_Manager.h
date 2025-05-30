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

	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[ENUM_CLASS(CHANNELID::SOUND_END)];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

private:
	void		LoadSoundFile();

public:
	static CSound_Manager* Create();
	virtual void Free() override;


};

END