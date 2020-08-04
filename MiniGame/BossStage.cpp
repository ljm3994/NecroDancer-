#include "stdafx.h"
#include "BossStage.h"


BossStage::BossStage()
{
}


BossStage::~BossStage()
{
}

HRESULT BossStage::init()
{
	MAPMANAGER->LoadMap(_T("BossStage.map"), "BossFloor");
	CHARTERMANAGER->EnemyReset();
	m_Player = CHARTERMANAGER->GetPlayer();
	m_Player->Setm_Pos(D3DXVECTOR2(13, 6));
	m_Player->Setm_PlayerSight(2);
	m_Player->Setm_ScreenPos(D3DXVECTOR2(m_Player->Getm_Pos().x * 50, m_Player->Getm_Pos().y * 50));
	BEATMANAGER->EnergieCalculation();
	BEATMANAGER->BpmCalculation();
	BEATMANAGER->BeatCalculation();
	UIMANAGER->Init();
	m_BossEnter = false;
	m_ScreenOn = true;
	FirstSoundOn = false;
	m_IsFirst = true;
	ScreenTop = D3DXVECTOR2(-270.0f, WINSIZEY - 120);
	ScreenMid = D3DXVECTOR2(WINSIZEX + 200, (WINSIZEY / 2) + 50);
	ScreenBot = D3DXVECTOR2(WINSIZEX + 400, (WINSIZEY / 2) - 280);
	CHARTERMANAGER->CreateJombe(8, D3DXVECTOR2(7, 13));
	CHARTERMANAGER->CreateJombe(8, D3DXVECTOR2(15, 13));
	CHARTERMANAGER->BossCreate();
	NamePos = D3DXVECTOR2(ScreenBot.x - 100, ScreenBot.y);
	m_ScreenAlpha = 1.0f;
	g_ptCam.x = (CHARTERMANAGER->GetPlayer()->Getm_Pos().x * 50) - (WINSIZEX / 2);
	g_ptCam.y = (CHARTERMANAGER->GetPlayer()->Getm_Pos().y * 50) - (WINSIZEY / 2);
	m_ScreenOut = false;
	m_ScreenDelay = 0.0f;
	m_LineDelay = 0.0f;
	SOUNDMANAGER->play("BossAnnouncer", GAMEMANAGER->GetsfxVolum());
	return S_OK;
}

void BossStage::release()
{
}

void BossStage::update()
{
	UIMANAGER->Update();
	if (m_ScreenOn)
	{
		ScreenUpdate();
		m_Player->PlayerRayCast();
		MAPMANAGER->GetAllMap()->TerianAlphaUpdate();
	}
	else
	{
		if (m_IsFirst)
		{
			SOUNDMANAGER->stop("BossAnnouncer");
			SOUNDMANAGER->play(BEATMANAGER->GetKey(), 0.0f);
			m_IsFirst = false;
		}
		MAPMANAGER->MapUpdate();
		CHARTERMANAGER->PlayerUpdate();
		if (MAPMANAGER->GetAllMap()->GetTile(12, 11)->Object == TileObject::OBJECT_EMPTY
			|| MAPMANAGER->GetAllMap()->GetTile(11, 11)->Object == TileObject::OBJECT_EMPTY
			|| MAPMANAGER->GetAllMap()->GetTile(13, 11)->Object == TileObject::OBJECT_EMPTY)
		{
			MAPMANAGER->GetAllMap()->ObjectsetMap(12, 11, 0, 0);
			MAPMANAGER->GetAllMap()->ObjectsetMap(11, 11, 0, 0);
			MAPMANAGER->GetAllMap()->ObjectsetMap(13, 11, 0, 0);
			if (!FirstSoundOn)
			{
				SOUNDMANAGER->stop(BEATMANAGER->GetKey());
				SOUNDMANAGER->play(BEATMANAGER->GetKey(), GAMEMANAGER->GetMusicVolum());
				FirstSoundOn = true;
			}
		}
		if (m_Player->Getm_Pos().y == 12 && !m_BossEnter)
		{
			MAPMANAGER->GetAllMap()->ObjectsetMap(12, 11, 0, 0, MODE_DRAW, "BossWall", false, NONPASS, OBJECT_WALL);
			MAPMANAGER->GetAllMap()->ObjectsetMap(11, 11, 0, 0, MODE_DRAW, "BossWall", false, NONPASS, OBJECT_WALL);
			MAPMANAGER->GetAllMap()->ObjectsetMap(13, 11, 0, 0, MODE_DRAW, "BossWall", false, NONPASS, OBJECT_WALL);
			LightChange();
			m_BossEnter = true;
		}
		if (m_BossEnter)
		{
			CHARTERMANAGER->JombeUpdate();
		}
	}
}

void BossStage::render()
{
	MAPMANAGER->MapRender();
	if (!m_BossEnter && m_Player->Getm_Pos().y >= 8)
	{
		BossLineRender();
		m_Player->render();
	}
	CHARTERMANAGER->Render();
	UIMANAGER->Render();
	if (m_ScreenOn)
	{
		ScreenRender();
	}
	if (m_BossEnter)
	{
		CHARTERMANAGER->Render();
	}
}

void BossStage::LightChange()
{
	MAPMANAGER->MapLightOff(0, 0, 30, 9);
}

void BossStage::ScreenRender()
{
	Image * img = IMGMANAGER->GetImage("TopBlade");
	img->Scale(WINSIZEX - 100, 100);
	img->Position(ScreenTop.x + g_ptCam.x, ScreenTop.y + g_ptCam.y);
	img->Render();
	img = IMGMANAGER->GetImage("BgGradient");
	img->Scale(WINSIZEX, 440);
	img->Position(ScreenMid.x + g_ptCam.x, (ScreenMid.y - 50) + g_ptCam.y);
	img->Render();
	img = IMGMANAGER->GetImage("BossScreen");
	img->Scale(WINSIZEX, 550);
	img->Position(ScreenMid.x + g_ptCam.x, ScreenMid.y + g_ptCam.y);
	img->Render();
	img = IMGMANAGER->GetImage("BottomBlade");
	img->Scale(WINSIZEX - 100, 100);
	img->Position(ScreenBot.x + g_ptCam.x, ScreenBot.y + g_ptCam.y);
	img->Render();
	img = IMGMANAGER->GetImage("BossName");
	img->Scale(400, 80);
	img->Position(NamePos.x + g_ptCam.x, NamePos.y + g_ptCam.y);
	img->Render();
}

void BossStage::ScreenUpdate()
{
	if (!m_ScreenOut)
	{
		bool Return = false;
		Return = ScreenTopUpdate(1);
		Return = ScreenBottomUpdate(-1);
		Return = ScreenMidUpdate(-1);

		if (Return)
		{
			m_ScreenDelay += g_ETime;
			if (m_ScreenDelay > 3.0f)
			{
				m_ScreenOut = true;
				m_ScreenDelay = 0.0f;
			}
		}
	}
	else
	{
		bool Return = false;
		Return = ScreenTopUpdate(-1);
		Return = ScreenBottomUpdate(1);
		Return = ScreenMidUpdate(1);

		if (Return)
		{
			m_ScreenOn = false;
		}
	}
}

void BossStage::BossLineRender()
{
	Image *img = IMGMANAGER->GetImage("BossLine");
	img->Position(600, 500);
	img->Scale(100, 170);
	img->Color(0, 0, 0, m_ScreenAlpha);
	img->Render();
	m_LineDelay += g_ETime;

	if (m_LineDelay > 0.5f)
	{
		if (m_ScreenAlpha <= 0.0f)
		{
			m_ScreenAlpha = 1.0f;
		}
		else
		{
			m_ScreenAlpha = 0.0f;
		}
		m_LineDelay = 0.0f;
	}
}

bool BossStage::ScreenTopUpdate(int Dirvec)
{
	if (!m_ScreenOut)
	{
		ScreenTop.x += (Dirvec * 300) * g_ETime;
		if (ScreenTop.x >= (WINSIZEX / 2) - 200)
		{
			ScreenTop.x = (WINSIZEX / 2) - 200;
			return true;
		}
	}
	else
	{
		ScreenTop.x += (Dirvec * 300) * g_ETime;
		if (ScreenTop.x <= -700)
		{
			ScreenTop.x = -700;
			return true;
		}
	}
	return false;
}

bool BossStage::ScreenMidUpdate(int Dirvec)
{
	if (!m_ScreenOut)
	{
		ScreenMid.x += (Dirvec * 300) * g_ETime;
		if (ScreenMid.x <= (WINSIZEX / 2))
		{
			ScreenMid.x = (WINSIZEX / 2);
			return true;
		}
	}
	else
	{
		ScreenMid.x += (Dirvec * 300) * g_ETime;
		if (ScreenMid.x >= WINSIZEX + 600)
		{
			ScreenMid.x = (WINSIZEX) +600;
			return true;
		}
	}
	return false;
}

bool BossStage::ScreenBottomUpdate(int Dirvec)
{
	if (!m_ScreenOut)
	{
		ScreenBot.x += (Dirvec * 300) * g_ETime;
		NamePos.x = ScreenBot.x - 100;
		if (ScreenBot.x <= (WINSIZEX / 2) + 300)
		{
			ScreenBot.x = (WINSIZEX / 2) + 300;
			NamePos.x = ScreenBot.x - 100;
			return true;
		}
	}
	else
	{
		ScreenBot.x += (Dirvec * 300) * g_ETime;
		NamePos.x = ScreenBot.x - 100;
		if (ScreenBot.x >= (WINSIZEX) + 700)
		{
			ScreenBot.x = (WINSIZEX) + 700;
			NamePos.x = ScreenBot.x - 100;
			return true;
		}
	}
	return false;
}
