#include "stdafx.h"
#include "GameInfo.h"


GameInfo::GameInfo()
{
	StageLevel LevelTemp;
	LevelTemp.StageName = "LobbyLocalcoop";
	LevelTemp.XIndex = 16;
	LevelTemp.YIndex = 24;
	LevelTemp.LevelStage = 7;
	LevelTemp.m_IsUse = false;
	m_Level.emplace_back(LevelTemp);
	StageLevel LevelTemp2;
	LevelTemp2.XIndex = 16;
	LevelTemp2.YIndex = 20;
	LevelTemp2.LevelStage = 5;
	LevelTemp2.StageName = "LobbyBoss";
	LevelTemp2.m_IsUse = false;
	m_Level.emplace_back(LevelTemp2);
	StageLevel LevelTemp3;
	LevelTemp3.XIndex = 16;
	LevelTemp3.YIndex = 18;
	LevelTemp3.LevelStage = 6;
	LevelTemp3.StageName = "LobbyLeveleditor";
	LevelTemp3.m_IsUse = false;
	m_Level.emplace_back(LevelTemp3);
	StageLevel LevelTemp5;
	LevelTemp5.XIndex = 14;
	LevelTemp5.YIndex = 24;
	LevelTemp5.LevelStage = 0;
	LevelTemp5.StageName = "LobbyZone1";
	LevelTemp5.m_IsUse = true;
	m_Level.emplace_back(LevelTemp5);
	StageLevel LevelTemp6;
	LevelTemp6.XIndex = 14;
	LevelTemp6.YIndex = 22;
	LevelTemp6.LevelStage = 1;
	LevelTemp6.StageName = "LobbyZone2";
	LevelTemp6.m_IsUse = false;
	m_Level.emplace_back(LevelTemp6);
	StageLevel LevelTemp7;
	LevelTemp7.XIndex = 14;
	LevelTemp7.YIndex = 20;
	LevelTemp7.LevelStage = 2;
	LevelTemp7.StageName = "LobbyZone3";
	LevelTemp7.m_IsUse = false;
	m_Level.emplace_back(LevelTemp7);
	StageLevel LevelTemp8;
	LevelTemp8.XIndex = 14;
	LevelTemp8.YIndex = 18;
	LevelTemp8.LevelStage = 3;
	LevelTemp8.StageName = "LobbyZone4";
	LevelTemp8.m_IsUse = false;
	m_Level.emplace_back(LevelTemp8);
	StageLevel LevelTemp9;
	LevelTemp9.XIndex = 12;
	LevelTemp9.YIndex = 24;
	LevelTemp9.LevelStage = 8;
	LevelTemp9.StageName = "LobbyAllZone";
	LevelTemp9.m_IsUse = false;
	m_Level.emplace_back(LevelTemp9);
	StageLevel LevelTemp10;
	LevelTemp10.XIndex = 12;
	LevelTemp10.YIndex = 22;
	LevelTemp10.LevelStage = 9;
	LevelTemp10.StageName = "LobbyCharter";
	LevelTemp10.m_IsUse = false;
	m_Level.emplace_back(LevelTemp10);
	StageLevel LevelTemp11;
	LevelTemp11.XIndex = 12;
	LevelTemp11.YIndex = 20;
	LevelTemp11.LevelStage = 10;
	LevelTemp11.StageName = "LobbyTutorial";
	LevelTemp11.m_IsUse = false;
	m_Level.emplace_back(LevelTemp11);
	StageLevel LevelTemp12;
	LevelTemp12.XIndex = 12;
	LevelTemp12.YIndex = 18;
	LevelTemp12.LevelStage = 11;
	LevelTemp12.StageName = "LobbyExtra";
	LevelTemp12.m_IsUse = false;
	m_Level.emplace_back(LevelTemp12);
	StageLevel LevelTemp13;
	LevelTemp13.XIndex = 10;
	LevelTemp13.YIndex = 24;
	LevelTemp13.LevelStage = 12;
	LevelTemp13.StageName = "LobbyDailychallenge";
	LevelTemp13.m_IsUse = false;
	m_Level.emplace_back(LevelTemp13);
	StageLevel LevelTemp14;
	LevelTemp14.XIndex = 10;
	LevelTemp14.YIndex = 18;
	LevelTemp14.LevelStage = 13;
	LevelTemp14.StageName = "LobbyAllZoneModeSeed";
	LevelTemp14.m_IsUse = false;
	m_Level.emplace_back(LevelTemp14);
	sfxVolum = 1.0f;
	MusicVolum = 0.5f;
	m_WindowWidth = 1200.0f;
	m_WindowHeight = 800.0f;
	m_CurrentLevel = -1;
	m_Mode = MODE_STORY;
	m_SkinName = "Player";
	m_LobbyBoost = false;
}


GameInfo::~GameInfo()
{
}

vector<StageLevel>& GameInfo::GetLevel()
{
	return m_Level;
}

StageLevel GameInfo::FindStage(int X, int Y)
{
	vector<StageLevel>::iterator it;
	it = find_if(m_Level.begin(), m_Level.end(), [&](StageLevel le) {
		return (le.XIndex == X && le.YIndex == Y); });
	
	if (it != m_Level.end())
	{
		return *it;
	}
	StageLevel Temp;
	Temp.LevelStage = -1;
	return Temp;
}
