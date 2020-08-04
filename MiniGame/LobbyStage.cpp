#include "stdafx.h"
#include "LobbyStage.h"
#include "Map/Shop.h"

LobbyStage::LobbyStage()
{
}


LobbyStage::~LobbyStage()
{
}

HRESULT LobbyStage::init()
{
	MAPMANAGER->LoadMap(_T("LobbyStage.map"), "Floor0");
	m_Pause = false;
	m_Player = CHARTERMANAGER->GetPlayer();
	m_Player->Setm_Pos(D3DXVECTOR2(13, 26));
	m_Player->Setm_PlayerSight(5);
	m_Player->Setm_Alive(true);
	m_Player->SetCurrentTile(MAPMANAGER->GetAllMap()->GetTile(13, 26));
	CHARTERMANAGER->GetPlayer()->Setm_Life(CHARTERMANAGER->GetPlayer()->Getm_MaxLife());
	m_Player->Setm_ScreenPos(D3DXVECTOR2(m_Player->Getm_Pos().x * 50, m_Player->Getm_Pos().y * 50));
	BEATMANAGER->BeatClear();
	BEATMANAGER->Init("LobbySound");
	BEATMANAGER->EnergieCalculation();
	BEATMANAGER->BpmCalculation();
	BEATMANAGER->BeatCalculation();
	UIMANAGER->Init();
	MAPMANAGER->Getm_Shop()->Init();
	BoostOptionRender = false;
	m_BoostKeyDown = false;
	SelectNum = 0;
	SOUNDMANAGER->play(BEATMANAGER->GetKey(), GAMEMANAGER->GetMusicVolum());
	g_ptCam.x = (CHARTERMANAGER->GetPlayer()->Getm_Pos().x * 50) - (WINSIZEX / 2);
	g_ptCam.y = (CHARTERMANAGER->GetPlayer()->Getm_Pos().y * 50) - (WINSIZEY / 2);
	SOUNDMANAGER->play("LobbySound", GAMEMANAGER->GetMusicVolum());
	return S_OK;
}

void LobbyStage::release()
{
}

void LobbyStage::update()
{
	if (!UIMANAGER->Getm_OptionUI()->Getm_Pause())
	{
		LobbyRayCast(10, 26);
		LobbyRayCast(16, 26);
		LobbyRayCast(10, 20);
		LobbyRayCast(16, 20);
		LobbyRayCast(10, 16);
		LobbyRayCast(16, 16);
		vector<StageLevel> Level = GAMEMANAGER->GetLevel();
		vector<StageLevel>::iterator it = Level.begin();
		for (; it != Level.end(); it++)
		{
			if (it->m_IsUse)
			{
				MAPMANAGER->GetAllMap()->ObjectsetMap(it->XIndex, it->YIndex, 0, 0, MODE_DRAW, "Stairs", false, PASS, OBJECT_STAIRS);
			}
			else
			{
				MAPMANAGER->GetAllMap()->ObjectsetMap(it->XIndex, it->YIndex, 0, 0, MODE_DRAW, "LockStairs", false, PASS, OBJECT_LOCKSTAIRS);
			}
		}
		MAPMANAGER->Getm_Shop()->Update();
		MAPMANAGER->MapUpdate();
		if (!BoostOptionRender)
		{
			CHARTERMANAGER->PlayerUpdate();
		}
		if (!GAMEMANAGER->Getm_LobbyBoost())
		{
			UIMANAGER->Getm_BeatUI()->update();
		}
		else
		{
			CHARTERMANAGER->GetPlayer()->Setm_BeatTime(true);
		}
		if (KEYMANAGER->StayKeyDown(VK_LEFT) && KEYMANAGER->StayKeyDown(VK_RIGHT))
		{
			if (!m_BoostKeyDown)
			{
				BoostOptionRender = true;
				m_BoostKeyDown = true;
			}
		}
		else
		{
			m_BoostKeyDown = false;
		}
		UIMANAGER->Getm_PlayerUI()->update();
		StageLevel Temp = GAMEMANAGER->FindStage(static_cast<int>(m_Player->Getm_Pos().x), static_cast<int>(m_Player->Getm_Pos().y));

		if (Temp.LevelStage > -1 && Temp.m_IsUse)
		{
			SOUNDMANAGER->stop("LobbySound");
			if (Temp.LevelStage >= 0 && Temp.LevelStage <= 4)
			{
				GAMEMANAGER->Setm_Mode(MODE_STORY);
				GAMEMANAGER->Setm_CurrentLevel(Temp.LevelStage);
			}
			else if (Temp.LevelStage == 5)
			{
				GAMEMANAGER->Setm_Mode(MODE_STORY);
				SCENEMANAGER->ChangeScene("BossStage");
				GAMEMANAGER->Setm_CurrentLevel(Temp.LevelStage);
				return;
			}
			else if(Temp.LevelStage == 8)
			{
				GAMEMANAGER->Setm_Mode(MODE_INFINITY);
				GAMEMANAGER->Setm_CurrentLevel(0);
			}
			
			SCENEMANAGER->ChangeScene("Loading");
		}
		if (BoostOptionRender)
		{
			if (KEYMANAGER->isKeyDown(VK_UP))
			{
				SelectNum--;
				if (SelectNum < 0)
				{
					SelectNum = 1;
				}
			}
			else if (KEYMANAGER->isKeyDown(VK_DOWN))
			{
				SelectNum++;
				if (SelectNum > 1)
				{
					SelectNum = 0;
				}
			}
			else if (KEYMANAGER->isKeyDown(VK_RETURN))
			{
				if (SelectNum == 0)
				{
					GAMEMANAGER->Setm_LobbyBoost(true);
				}
				else
				{
					GAMEMANAGER->Setm_LobbyBoost(false);
				}
				BoostOptionRender = false;
			}
		}
		if (KEYMANAGER->isKeyDown(VK_ESCAPE))
		{
			UIMANAGER->Getm_OptionUI()->Setm_Pause(true);
			SOUNDMANAGER->pause(BEATMANAGER->GetKey());
		}
	}
	else
	{
		UIMANAGER->Update();
	}
}

void LobbyStage::render()
{
	if (!UIMANAGER->Getm_OptionUI()->Getm_Pause())
	{
		MAPMANAGER->MapRender();
	
		if (!GAMEMANAGER->Getm_LobbyBoost())
		{
			UIMANAGER->Getm_BeatUI()->render();
		}
		UIMANAGER->Getm_PlayerUI()->render();

		if (BoostOptionRender)
		{
			Image * img;
			img = IMGMANAGER->GetImage("FastMove1");
			img->Scale(350, 50);
			img->Position(350 + g_ptCam.x, 500 + g_ptCam.y);
			img->Render();
			img = IMGMANAGER->GetImage("FastMove2");
			img->Scale(200, 50);
			img->Position(650 + g_ptCam.x, 500 + g_ptCam.y);
			img->Render();
			Image * OkIMG;
			Image * NoIMG;

			if (SelectNum == 0)
			{
				OkIMG = IMGMANAGER->GetImage("OptionOkSelect");
				NoIMG = IMGMANAGER->GetImage("OptionNo");
			}
			else
			{
				OkIMG = IMGMANAGER->GetImage("OptionOk");
				NoIMG = IMGMANAGER->GetImage("OptionNoSelect");
			}
			OkIMG->Scale(100, 50);
			OkIMG->Position((WINSIZEX / 2) + g_ptCam.x, 300 + g_ptCam.y);
			OkIMG->Render();
			NoIMG->Scale(100, 50);
			NoIMG->Position((WINSIZEX / 2) + g_ptCam.x, 200 + g_ptCam.y);
			NoIMG->Render();
		}
	}
	else
	{
		UIMANAGER->Render();
	}
}

void LobbyStage::LobbyRayCast(int xpos, int ypos)
{
	float lay = 0.0f;
	float max = static_cast<float>(10) - 0.25f;

	float bright = 0.0f;
	float sightmax = 5.0f;
	float CurrentSight = 0.0f;

	float r = 0.0f;
	float angle = (15.0f * PI) / 180.0f;

	while (r < 2 * PI)
	{
		if (lay > max)
			bright = 0.0f;
		else
			bright = lay;

		if (CurrentSight > sightmax)
		{
			CurrentSight = 0.0f;
			bright = 0.0f;
			lay = 0.0f;
			r += angle;
		}

		float X = xpos + CurrentSight * cos(r) + 0.5f;
		float y = ypos + CurrentSight * -sin(r) + 0.5f;

		if (X < 0 || y < 0)
		{
			CurrentSight = sightmax + 0.1f;
			continue;
		}
		if (X > 99 || y > 99)
		{
			CurrentSight = sightmax + 0.1f;
			continue;
		}
		int a = static_cast<int>(X - xpos);
		if (a < 0)
		{
			a *= -1;
		}
		int b = static_cast<int>(y - ypos);
		if (b < 0)
		{
			b *= -1;
		}
		int c = a + b;

		int val = MAPMANAGER->PlayerRayUpdate(static_cast<int>(X), static_cast<int>(y), c, 10);

		if (val == 1)
		{
			lay = 0.0f;
			CurrentSight = 0.0f;
			r += angle;
		}
		else if (val == 2)
		{
			lay += 0.55f;
			CurrentSight += 0.25f;
		}
	}
}
