#pragma once
#include "SingletonBase.h"
struct StageLevel
{
	string StageName;
	int LevelStage;
	int XIndex;
	int YIndex;
	string ImageKey;
	bool m_IsUse;
};

enum GAMEMODE
{
	MODE_INFINITY,
	MODE_STORY
};
class GameInfo : public SingletonBase<GameInfo>
{
	vector<StageLevel>  m_Level;
	GAMEMODE m_Mode;
	string m_SkinName;
	bool m_LobbyBoost;
	int m_CurrentLevel;
	float m_WindowWidth;
	float m_WindowHeight;
	float sfxVolum;
	float MusicVolum;
public:
	GameInfo();
	~GameInfo();
	vector<StageLevel>& GetLevel();
	StageLevel FindStage(int X, int Y);
	MAKEGETSET(float, sfxVolum);
	MAKEGETSET(float, MusicVolum);
	MAKEGETSET(int, m_CurrentLevel);
	MAKEGETSET(bool, m_LobbyBoost);
	MAKEGETSET(string, m_SkinName);
	MAKEGETSET(GAMEMODE, m_Mode);
};

