#pragma once
#include "GameNode.h"
class FFTDetector;
enum NoteState
{
	NOTEALIVE,
	NOTEDIE
};
struct Note
{
	Image * NoteImg;
	D3DXVECTOR2 m_pos;
	NoteState State;
	float a;
};

struct MissImg
{
	D3DXVECTOR2 m_Pos;
	string ImgKey;
	float Alpha;
};

class WinUI : public GameNode
{
	Image * Heart;
	string strKey;
	FFTDetector * detect;
	vector<Note*> LeftNote;
	vector<Note*> RightNote;
	vector<MissImg*> m_MissImg;
	vector<Note*>::iterator LeftNoteit;
	vector<Note*>::iterator RightNoteit;
	vector<MissImg*>::iterator ImgIt;
	bool m_isBeat;
	bool m_PerfectBeat;
	bool m_isFirst;
	FRECT m_Pan;
	int m_Lenght;
	int CurrentFramX;
	float BetteweenTime;
	int m_Tempo;
	bool m_PlayerKeyCheck;
	float * Beat;
	float m_NoteLostDelay;
	float PrevGptCamX;
public:
	WinUI();
	~WinUI();
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void SetKey(string Key);
	void FFTWindowRender();
	MAKEGET(bool, m_isBeat);
	MAKEGET(bool, m_PerfectBeat);
	MAKEGETSET(bool, m_PlayerKeyCheck);
};

