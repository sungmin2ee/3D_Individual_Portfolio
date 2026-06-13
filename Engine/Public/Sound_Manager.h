#pragma once
#include "Engine_Defines.h"

// 전방선언
// 클라에서 fmod 헤더 노출안시키기위해
struct FMOD_SOUND;
struct FMOD_CHANNEL;
struct FMOD_SYSTEM;

NS_BEGIN(Engine)

class  CSound_Manager
{

private:
	CSound_Manager();
public:
	virtual ~CSound_Manager();



public:
	HRESULT Initialize();

	FMOD_SYSTEM* Get_System() const { return m_pSystem; }

public:
	void PlaySoundLoop(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume);
	void PlaySoundLoop(const TCHAR* soundKey,FMOD_CHANNEL** ppChannel,_float volume);
	void PlaySoundOne(const TCHAR* soundKey, FMOD_CHANNEL** ppChannel, _float volume);

	void PlaySoundOne(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume);
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, _float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, _float fVolume);
	void StopSound(CHANNELID eID);
	void StopSound(FMOD_CHANNEL** ppChannel);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, _float fVolume);
	void SetChannelVolume(FMOD_CHANNEL** ppChannel, _float fVolume);
	void UpdateSound();

private:
	void LoadSoundFile();
	void Release();

private:

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

public:
	static unique_ptr<CSound_Manager> Create();

};

NS_END