#pragma once
#include "GameNode.h"
#include <atlbase.h>
#include <ShObjIdl.h>

class FFTDetector;
class OptionUI : public GameNode
{
	int m_SelectNum;
	int m_MaxSelectNum;
	int m_SkinNum;
	vector<string> PlayerKeyVec;
	FFTDetector * Detector;
	string Path;
	bool m_FFTStart;
	bool m_FileOpen;
	int SelectNum;
	CComPtr<IFileDialog> pfd;
	bool m_AudioOption;
	bool m_SkinChangeOption;
	bool m_OptionRender;
	bool m_FFTOption;
	bool m_Pause;
	bool m_LobbyGo;
	bool m_QuickRestart;
	float m_SelectMusicVol;
	float m_SelectsfxVol;
	float m_MusicVolum;
	float m_SfxVolum;
	void PauseOptionRender();
	void OptionRender();
	void AudioOptionRender();
	void SkinChangeOptionRender();
	void FFTOptionRender();
public:
	OptionUI();
	~OptionUI() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	MAKEGETSET(bool, m_Pause);
	MAKEGET(bool, m_LobbyGo);
	MAKEGET(bool, m_QuickRestart);
};

