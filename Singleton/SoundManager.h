#pragma once
#include "SingletonBase.h"

#pragma comment (lib, "fmod/fmodex_vc.lib")
#include "fmod/fmod.hpp"

using namespace FMOD;
//ä�ι��� �� ���� ����
#define EXTRACHANNERBUFFER 5
#define SOUNDBUFFER 100

//�� ���� ���� ���� : ���۰� �۾Ƽ� ������ ������ �ذ�
#define TOTALSOUNDBUFFER SOUNDBUFFER * EXTRACHANNERBUFFER

class SoundManager : public SingletonBase<SoundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;
	typedef map<string, Channel*> arrChannel;
	typedef map<string, Channel*>::iterator arrChannelIter;
	System* m_system;		//fmod�� �ý��� Ŭ����
	Sound** m_sound;		//fmod�� ���� Ŭ����
	Channel** m_channel;	//fmod�� ä�� Ŭ����
	arrChannel m_totalChannel;
	arrSounds m_totalSound;	//�ʿ� ��� ���� ���
	float m_Pitch;

public:
	SoundManager();
	~SoundManager();
	//���� �߰�(Ű��, �����̸�, ����?)
	void addSound(string keyName, string soundName, bool loop = false);
	//���� �÷���(Ű��, ����) 0.0~1.0
	void play(string keyName, float volume = 1.0f);
	//���� ����
	void stop(string keyName);
	//���� �Ͻ�����
	void pause(string keyName);
	//���� �ٽ� ���
	void resume(string keyName);
	//�÷��� ������?
	void SoundVolumChange(string KeyName, float Volum);
	bool isPlaySound(string keyName);
	//�Ͻ����� ������?
	bool isPauseSound(string keyName);
	bool GetSytem(System*& system);
	bool GetAudioSystem(string KeyName, Sound *& Audio, Channel *& AudioChannel);
	UINT GetLength(string KeyName);
	bool GetPCMData(string KeyName, int *& RightData, int *& LeftData);
	bool GetPitch(string KeyName, float &Pitch);
	bool GetCurrentTimePos(string strKey, UINT & Pos);
	bool GetCurrentMSTimePos(string strKey, UINT & Pos);
	bool GetSpectrum(string strKey, float *& LeftData, float *& RightData, int SampleSize);
	bool GetSpectrum(string strKey, vector<float>& spectrum, int Size);
	//������Ʈ ��� ���������
	void Update();
};

