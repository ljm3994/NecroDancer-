#include "stdafx.h"
#include "DieUI.h"


DieUI::DieUI()
{
}


DieUI::~DieUI()
{
}

HRESULT DieUI::init()
{
	m_RestartButtonOn = false;
	m_Select = 0;
	MaxSelect = 1;
	return S_OK;
}

void DieUI::release()
{
}

void DieUI::update()
{
	if (!m_RestartButtonOn)
	{
		if (KEYMANAGER->isKeyDown(VK_UP) && KEYMANAGER->isKeyDown(VK_DOWN))
		{
			m_RestartButtonOn = true;
		}
	}
	else
	{
		if (KEYMANAGER->isKeyDown(VK_UP))
		{
			if (m_Select == 0)
			{
				m_Select = MaxSelect;
			}
			else
			{
				m_Select--;
			}
		}
		else if (KEYMANAGER->isKeyDown(VK_DOWN))
		{
			if (m_Select == MaxSelect)
			{
				m_Select = 0;
			}
			else
			{
				m_Select++;
			}
		}
		else if (KEYMANAGER->isKeyDown(VK_RETURN))
		{
			switch (m_Select)
			{
			case 0:
				SOUNDMANAGER->stop(BEATMANAGER->GetKey());
				SCENEMANAGER->ChangeScene("Loading");
				break;
			case 1:
				SOUNDMANAGER->stop(BEATMANAGER->GetKey());
				GAMEMANAGER->Setm_CurrentLevel(-1);
				SCENEMANAGER->ChangeScene("Loading");
				break;
			}
		}
	}
}

void DieUI::render()
{
	Image * img;
	img = IMGMANAGER->GetImage("PopupBack");
	img->Scale(WINSIZEX, WINSIZEY);
	img->Position(WINSIZEX / 2 + g_ptCam.x, WINSIZEY / 2 + g_ptCam.y);
	img->Color(0, 0, 0, 0.4f);
	img->Render();
	if (!m_RestartButtonOn)
	{
		img = IMGMANAGER->GetImage("DeathFont1");
		img->Scale(350, 50);
		img->Position(350 + g_ptCam.x, 200 + g_ptCam.y);
		img->Render();
		img = IMGMANAGER->GetImage("UpDown");
		img->Scale(200, 50);
		img->Position(650 + g_ptCam.x, 200 + g_ptCam.y);
		img->Render();
		img = IMGMANAGER->GetImage("DeathFont2");
		img->Scale(200, 50);
		img->Position(900 + g_ptCam.x, 200 + g_ptCam.y);
		img->Render();
	}
	else
	{
		img = IMGMANAGER->GetImage("Runsummary");
		img->Scale(350, 60);
		img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 100) + g_ptCam.y);
		img->Render();
		img = IMGMANAGER->GetImage("Killedby");
		img->Scale(200, 50);
		img->Position(((WINSIZEX / 2) - 300) + g_ptCam.x, ((WINSIZEY / 2) + 150) + g_ptCam.y);
		img->Render();
		img = IMGMANAGER->GetImage("Score");
		img->Scale(200, 50);
		img->Position(((WINSIZEX / 2) - 300) + g_ptCam.x, ((WINSIZEY / 2)) + g_ptCam.y);
		img->Render();
		CoinRender();
		string QuickStart;
		string LobbReturn;
		switch (m_Select)
		{
		case 0:
			QuickStart = "QuickStartSelect";
			LobbReturn = "GameLobbyRe";
			break;
		case 1:
			QuickStart = "QuickStart";
			LobbReturn = "OptionLobbySelect";
			break;
		}

		img = IMGMANAGER->GetImage(QuickStart);
		img->Scale(100, 50);
		img->Position(((WINSIZEX / 2)) + g_ptCam.x, 250 + g_ptCam.y);
		img->Render();
		img = IMGMANAGER->GetImage(LobbReturn);
		img->Scale(100, 50);
		img->Position(((WINSIZEX / 2)) + g_ptCam.x, 180 + g_ptCam.y);
		img->Render();
	}
}

void DieUI::CoinRender()
{
	Player * player = CHARTERMANAGER->GetPlayer();
	int CurrentCoin = player->Getm_Coin();

	Image * NumImg = IMGMANAGER->GetImage("greyfont");
	NumImg->Scale(30, 30);

	int Num[4] = { 0, 0, 0, 0 };
	Num[0] = CurrentCoin / 1000;
	Num[1] = (CurrentCoin - Num[0] * 1000) / 100;
	Num[2] = (CurrentCoin - Num[0] * 1000 - Num[1] * 100) / 10;
	Num[3] = (CurrentCoin - Num[0] * 1000 - Num[1] * 100 - Num[2] * 10) / 1;
	bool First = false;
	for (int i = 0; i < 4; i++)
	{
		if (First || i == 3)
		{
			NumImg->Position((((WINSIZEX / 2) - 100) + (50 * i)) + g_ptCam.x, ((WINSIZEY / 2)) + g_ptCam.y);
			if (Num[i] == 0)
			{
				Num[i] = 9;
			}
			NumImg->GetRefCurrentFrameX() = Num[i];
		}
		else if (First == false && Num[i] != 0)
		{
			NumImg->Position((((WINSIZEX / 2) - 100) + (50 * i)) + g_ptCam.x, ((WINSIZEY / 2)) + g_ptCam.y);
			NumImg->GetRefCurrentFrameX() = Num[i];
			First = true;
		}
		NumImg->Render();
	}
}
