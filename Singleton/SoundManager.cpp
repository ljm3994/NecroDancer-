#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
	:m_system(nullptr)
	, m_channel(nullptr)
	, m_sound(nullptr)
{
	//사운드 시스템 생성
	System_Create(&m_system);

	//사운드 채널수 생성
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 동적할당
	m_sound = new Sound *[TOTALSOUNDBUFFER];
	m_channel = new Channel *[TOTALSOUNDBUFFER];

	m_Pitch = 1.0f;
	memset(m_sound, 0, sizeof(Sound*)* (TOTALSOUNDBUFFER));
	memset(m_channel, 0, sizeof(Channel*)* (TOTALSOUNDBUFFER));
}


SoundManager::~SoundManager()
{
	if (m_channel != nullptr || m_sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_channel != nullptr)
			{
				if (m_channel[i]) m_channel[i]->stop();
			}

			if (m_sound != nullptr)
			{
				if (m_sound[i]) m_sound[i]->release();
			}
		}
	}

	SAFE_DELETE(m_channel);
	SAFE_DELETE(m_sound);

	//시스템 닫기
	if (m_system != nullptr)
	{
		m_system->release();
		m_system->close();
	}
	//맵 비우기
	m_totalSound.clear();
	m_totalChannel.clear();
}

void SoundManager::addSound(string keyName, string soundName, bool loop)
{
	if (loop)
	{
		m_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSound.size()]);
	}
	else
	{
		m_system->createSound(soundName.c_str(), FMOD_DEFAULT, nullptr, &m_sound[m_totalSound.size()]);
	}
	
	//맵에 사운드를 키값과 함께 담아준다.
	m_totalSound.insert(make_pair(keyName, &m_sound[m_totalSound.size()]));
	m_totalChannel.insert(make_pair(keyName, m_channel[m_totalSound.size()]));
}

void SoundManager::play(string keyName, float volume)
{
	bool isPlay;
	int count = 0;
	//기존 사운드 재생중인지 확인
	m_totalChannel[keyName]->isPlaying(&isPlay);
	if (isPlay)return;
	//사운드 플레이
	m_system->playSound(FMOD_CHANNEL_FREE, *m_totalSound[keyName], false, &m_totalChannel[keyName]);
	//볼륨셋팅
	m_totalChannel[keyName]->setVolume(volume);
}

void SoundManager::stop(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	//기존 사운드 재생중인지 확인
	m_totalChannel[keyName]->isPlaying(&isPlay);
	if (!isPlay)return;
	m_totalChannel[keyName]->stop();
}

void SoundManager::pause(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	m_totalChannel[keyName]->getPaused(&isPause);
	if (isPause)return;
	m_totalChannel[keyName]->setPaused(true);
}

void SoundManager::resume(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	m_totalChannel[keyName]->getPaused(&isPause);
	if (!isPause)return;
	m_totalChannel[keyName]->setPaused(false);
}

void SoundManager::SoundVolumChange(string KeyName, float Volum)
{
	bool isPause = false;
	m_totalChannel[KeyName]->isPlaying(&isPause);
	if (!isPause)return;
	m_totalChannel[KeyName]->setVolume(Volum);
}

bool SoundManager::isPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	m_totalChannel[keyName]->isPlaying(&isPlay);
	return isPlay;
}

bool SoundManager::isPauseSound(string keyName)
{
	return false; bool isPause;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	m_totalChannel[keyName]->getPaused(&isPause);

	return isPause;
}
bool SoundManager::GetSytem(System *& system)
{
	system = m_system;
	return true;
}
bool SoundManager::GetAudioSystem(string KeyName, Sound *& Audio, Channel *& AudioChannel)
{
	arrSoundIter iter = m_totalSound.begin();
	int count = 0;

	AudioChannel = m_totalChannel[KeyName];
	Audio = *m_totalSound[KeyName];
	return true;
}

UINT SoundManager::GetLength(string KeyName)
{
	arrSoundIter iter = m_totalSound.begin();
	int count = 0;
	UINT Length = 0;

	(*m_totalSound[KeyName])->getLength(&Length, FMOD_TIMEUNIT_PCM);

	return Length;
}

bool SoundManager::GetPCMData(string KeyName, int *& RightData, int *& LeftData)
{
	void * ptr1;
	void * ptr2;
	UINT Length1;
	UINT Length2;
	int ByteNum = 16;
	arrSoundIter iter = m_totalSound.begin();
	int count = 0;
	UINT Length = 0;
	int channels, bits;
	FMOD_SOUND_FORMAT format;
	FMOD_SOUND_TYPE type;

	(*m_totalSound[KeyName])->getLength(&Length, FMOD_TIMEUNIT_PCM);
	RightData = new int[Length];
	LeftData = new int[Length];

	(*m_totalSound[KeyName])->getFormat(&type, &format, &channels, &bits);
	ByteNum = bits;
	FMOD_RESULT fr;

	fr = (*m_totalSound[KeyName])->lock(0, Length, &ptr1, &ptr2, &Length1, &Length2);
	
	if (fr == FMOD_OK)
	{
		for (int i = 0; i < static_cast<int>(Length); i++)
		{
			RightData[i] = (static_cast<int*>(ptr1)[i] << ByteNum) >> ByteNum;
			LeftData[i] = (static_cast<int*>(ptr1)[i] >> ByteNum);
		}

		(*m_totalSound[KeyName])->unlock(ptr1, ptr2, Length, Length2);
		return true;
	}
	return false;
}

bool SoundManager::GetPitch(string KeyName, float & Pitch)
{
	Pitch = m_Pitch;
	return true;
}

bool SoundManager::GetCurrentTimePos(string strKey, UINT & Pos)
{
	arrSoundIter iter = m_totalSound.begin();
	int count = 0;
	UINT Length = 0;
	bool isPlay = false;
	m_totalChannel[strKey]->isPlaying(&isPlay);
	if (!isPlay)return false;
	m_totalChannel[strKey]->getPosition(&Pos, FMOD_TIMEUNIT_PCM);
	return true;
}

bool SoundManager::GetCurrentMSTimePos(string strKey, UINT & Pos)
{
	arrSoundIter iter = m_totalSound.begin();
	int count = 0;
	UINT Length = 0;
	bool isPlay = false;

	m_totalChannel[strKey]->isPlaying(&isPlay);
	if (!isPlay)return false;
	m_totalChannel[strKey]->getPosition(&Pos, FMOD_TIMEUNIT_MS);
	return true;
}

bool SoundManager::GetSpectrum(string strKey, float *& LeftData, float *& RightData, int SampleSize)
{
	bool isPlay = false;

	m_totalChannel[strKey]->isPlaying(&isPlay);
	if (isPlay)
	{
		RightData = new float[SampleSize];
		LeftData = new float[SampleSize];
		m_totalChannel[strKey]->getSpectrum(RightData, SampleSize, 0, FMOD_DSP_FFT_WINDOW_HAMMING);
		m_totalChannel[strKey]->getSpectrum(LeftData, SampleSize, 1, FMOD_DSP_FFT_WINDOW_HAMMING);
		return true;
	}
	return false;
}

bool SoundManager::GetSpectrum(string strKey, vector<float>& spectrum, int Size)
{
	bool isPlay = false;

	m_totalChannel[strKey]->isPlaying(&isPlay);

	if (isPlay)
	{
		float* rightChannel = new float[Size];
		float* leftChannel = new float[Size];

		m_totalChannel[strKey]->getSpectrum(rightChannel, Size, 0, FMOD_DSP_FFT_WINDOW_HAMMING);
		m_totalChannel[strKey]->getSpectrum(leftChannel, Size, 1, FMOD_DSP_FFT_WINDOW_HAMMING);

		for (int i = 0; i < Size; i++)
		{
			spectrum.emplace_back((leftChannel[i] + rightChannel[i]) / 2);
		}


		// 가져오는 값은 데시벨 값으로 0~1사이의 값을 가져온다.
		// 가장 큰 주파수의 경우 항상 1로 고정 시키기 위한 알고리즘 사용
		float maxiterator = *max_element(spectrum.begin(), spectrum.end());  // 최대 주파수를 가져온다

		float MaxVolum = 1.0f / maxiterator;

		for (int i = 0; i < Size; i++)
		{
			spectrum[i] *= MaxVolum;
		}

		SAFE_DELETE_ARRAY(rightChannel);
		SAFE_DELETE_ARRAY(leftChannel);
		return true;
	}
	return false;
}

void SoundManager::Update()
{
	/*
	사운드 시스템을 계속 업데이트 해줘야 볼륨이 바뀌거나
	재생이 끝난 사운드를 채널에서 빼내는 등 다양한 작업을 자동으로 해준다.
	*/
	//메인게임에서 사운드 매니져를 업데이트 해준다.
	m_system->update();
}
