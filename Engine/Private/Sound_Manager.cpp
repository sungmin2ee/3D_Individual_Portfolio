#include "Sound_Manager.h"
#pragma comment(lib, "fmod_vc.lib")
CSound_Manager::CSound_Manager()
{
	m_pSystem = nullptr;
}


CSound_Manager::~CSound_Manager()
{
	Release();
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
void CSound_Manager::Release()
{
	for (auto& pair : m_mapSound)
	{
		delete[] pair.first;
		FMOD_Sound_Release(pair.second);
	}

	m_mapSound.clear();

	if (m_pSystem)
	{
		FMOD_System_Close(m_pSystem);
		FMOD_System_Release(m_pSystem);
		m_pSystem = nullptr;
	}
}
void CSound_Manager::PlaySoundLoop(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& pair)
		{
			return !lstrcmp(svSoundKey, pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(
		m_pSystem,
		iter->second,
		nullptr,
		FALSE,
		&m_pChannelArr[eID]);
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);

}
void CSound_Manager::PlaySoundLoop(const TCHAR* soundKey,FMOD_CHANNEL** ppChannel,_float volume)
{
	auto iter = find_if(
		m_mapSound.begin(),
		m_mapSound.end(),
		[&](auto& pair)
		{
			return !lstrcmp(soundKey, pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(
		m_pSystem,
		iter->second,
		nullptr,
		FALSE,
		ppChannel);

	FMOD_Channel_SetMode(*ppChannel, FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(*ppChannel, volume);
}
void CSound_Manager::PlaySoundOne(const TCHAR* soundKey,FMOD_CHANNEL** ppChannel,_float volume)
{
	auto iter = find_if(
		m_mapSound.begin(),
		m_mapSound.end(),
		[&](auto& pair)
		{
			return !lstrcmp(soundKey, pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(
		m_pSystem,
		iter->second,
		nullptr,
		FALSE,
		ppChannel);

	FMOD_Channel_SetVolume(*ppChannel, volume);
}

void CSound_Manager::PlaySoundOne(const TCHAR* svSoundKey, CHANNELID eID, _float fVolume)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& pair)
		{
			return !lstrcmp(svSoundKey, pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(
		m_pSystem,
		iter->second,
		nullptr,
		FALSE,
		&m_pChannelArr[eID]);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);

}


void CSound_Manager::PlaySound(const TCHAR* pSoundKey, CHANNELID eID, _float fVolume)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& pair)
		{
			return !lstrcmp(pSoundKey, pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay);

	// 재생 중이 아닐 때만 재생
	if (!bPlay)
	{
		FMOD_System_PlaySound(
			m_pSystem,
			iter->second,
			nullptr,
			FALSE,
			&m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlayBGM(const TCHAR* pSoundKey, _float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(
		m_pSystem,
		iter->second,
		nullptr,
		FALSE,
		&m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}
void CSound_Manager::StopSound(FMOD_CHANNEL** ppChannel)
{
	FMOD_Channel_Stop(*ppChannel);
}

void CSound_Manager::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::SetChannelVolume(CHANNELID eID, _float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}
void CSound_Manager::SetChannelVolume(FMOD_CHANNEL** ppChannel, _float fVolume)
{
	FMOD_Channel_SetVolume(*ppChannel, fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata64i32_t fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t  handle = _findfirst("../../Resources/Sound/*.wav", &fd);
	if (handle == -1) return;

	int iResult = 0;

	char szCurPath[128] = "../../Resources/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../ Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(
			m_pSystem,
			szFullPath,
			FMOD_DEFAULT,
			nullptr,
			&pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.insert({ pSoundKey, pSound });
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 
		// 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}
void CSound_Manager::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}

unique_ptr<CSound_Manager> CSound_Manager::Create()
{

	auto		pInstance = unique_ptr<CSound_Manager>(new CSound_Manager());

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : SaveLoad_Manager");
		return nullptr;
	}

	return pInstance;
}

