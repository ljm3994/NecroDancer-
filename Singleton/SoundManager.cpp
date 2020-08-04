#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
	:m_system(nullptr)
	, m_channel(nullptr)
	, m_sound(nullptr)
{
	//���� �ý��� ����
	System_Create(&m_system);

	//���� ä�μ� ����
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//���� ä�� �����Ҵ�
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

	//�ý��� �ݱ�
	if (m_system != nullptr)
	{
		m_system->release();
		m_system->close();
	}
	//�� ����
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
	
	//�ʿ� ���带 Ű���� �Բ� ����ش�.
	m_totalSound.insert(make_pair(keyName, &m_sound[m_totalSound.size()]));
	m_totalChannel.insert(make_pair(keyName, m_channel[m_totalSound.size()]));
}

void SoundManager::play(string keyName, float volume)
{
	bool isPlay;
	int count = 0;
	//���� ���� ��������� Ȯ��
	m_totalChannel[keyName]->isPlaying(&isPlay);
	if (isPlay)return;
	//���� �÷���
	m_system->playSound(FMOD_CHANNEL_FREE, *m_totalSound[keyName], false, &m_totalChannel[keyName]);
	//��������
	m_totalChannel[keyName]->setVolume(volume);
}

void SoundManager::stop(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	//���� ���� ��������� Ȯ��
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


		// �������� ���� ���ú� ������ 0~1������ ���� �����´�.
		// ���� ū ���ļ��� ��� �׻� 1�� ���� ��Ű�� ���� �˰��� ���
		float maxiterator = *max_element(spectrum.begin(), spectrum.end());  // �ִ� ���ļ��� �����´�

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
	���� �ý����� ��� ������Ʈ ����� ������ �ٲ�ų�
	����� ���� ���带 ä�ο��� ������ �� �پ��� �۾��� �ڵ����� ���ش�.
	*/
	//���ΰ��ӿ��� ���� �Ŵ����� ������Ʈ ���ش�.
	m_system->update();
}
