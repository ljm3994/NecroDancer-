#include "stdafx.h"
#include "MainSceen.h"
#include "Map/Shop.h"

MainSceen::MainSceen()
{
}


MainSceen::~MainSceen()
{
}

HRESULT MainSceen::init()
{
	BEATMANAGER->EnergieCalculation();
	BEATMANAGER->BpmCalculation();
	BEATMANAGER->BeatCalculation();
	UIMANAGER->Init();
	m_Pause = false;
	SOUNDMANAGER->play(BEATMANAGER->GetKey(), GAMEMANAGER->GetMusicVolum());

	memcpy_s(&m_TempPlayer, sizeof(Player), CHARTERMANAGER->Getm_Player(), sizeof(Player));
	g_ptCam.x = (CHARTERMANAGER->GetPlayer()->Getm_Pos().x * 50) - (WINSIZEX / 2);
	g_ptCam.y = (CHARTERMANAGER->GetPlayer()->Getm_Pos().y * 50) - (WINSIZEY / 2);
	return S_OK;
}

void MainSceen::release()
{
}

void MainSceen::update()
{
	if (!UIMANAGER->Getm_OptionUI()->Getm_Pause())
	{
		if (m_Pause)
		{
			SOUNDMANAGER->resume(BEATMANAGER->GetKey());
			m_Pause = false;
		}
		MAPMANAGER->MapUpdate();
		CHARTERMANAGER->Update();
		UIMANAGER->Update();

		if (KEYMANAGER->isKeyDown(VK_ESCAPE))
		{
			UIMANAGER->Getm_OptionUI()->Setm_Pause(true);
		}
		MAPMANAGER->Getm_Shop()->Update();
	}
	else
	{
		if (!m_Pause)
		{
			SOUNDMANAGER->pause(BEATMANAGER->GetKey());
			m_Pause = true;
		}
		UIMANAGER->Update();
		if (UIMANAGER->Getm_OptionUI()->Getm_QuickRestart())
		{
			memcpy_s(CHARTERMANAGER->Getm_Player(), sizeof(Player), &m_TempPlayer, sizeof(Player));
			SOUNDMANAGER->stop(BEATMANAGER->GetKey());
			SCENEMANAGER->ChangeScene("Loading");
		}
		else if (UIMANAGER->Getm_OptionUI()->Getm_LobbyGo())
		{
			memcpy_s(CHARTERMANAGER->Getm_Player(), sizeof(Player), &m_TempPlayer, sizeof(Player));
			GAMEMANAGER->Setm_CurrentLevel(-1);
			SOUNDMANAGER->stop(BEATMANAGER->GetKey());
			SCENEMANAGER->ChangeScene("Loading");
		}
	}
}

void MainSceen::render()
{
	if (!UIMANAGER->Getm_OptionUI()->Getm_Pause())
	{
		MAPMANAGER->MapRender();
		UIMANAGER->Render();
	}
	else
	{
		UIMANAGER->Render();
	}
}

