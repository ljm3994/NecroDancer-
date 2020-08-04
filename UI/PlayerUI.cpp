#include "stdafx.h"
#include "PlayerUI.h"


PlayerUI::PlayerUI()
{
}


PlayerUI::~PlayerUI()
{
}

HRESULT PlayerUI::init()
{
	m_PlayerLife = 6;
	m_PlayerMaxLife = 6;
	m_Beat = 0;
	m_IsPlayerDie = false;
	m_IsInven[0] = true;
	m_IsInven[1] = true;
	m_IsInven[2] = false;
	m_IsInven[3] = false;
	m_IsInven[4] = false;
	m_IsPack = true;
	CurrentCoin = 0;
	m_IsTempo = false;
	CurrentDiamond = 0;
	m_DelayTime = 0.0f;
	return S_OK;
}

void PlayerUI::release()
{
}

void PlayerUI::update()
{
	if (m_IsTempo)
	{
		HpBeatOn();
		m_IsTempo = false;
	}
}

void PlayerUI::render()
{
	HpRender();
	CoinRender();
	InventoryRender();
}

void PlayerUI::HpRender()
{
	Player * player = CHARTERMANAGER->GetPlayer();
	m_PlayerMaxLife = player->Getm_MaxLife();
	m_PlayerLife = player->Getm_Life();

	for (int i = (m_PlayerMaxLife / 2) - 1; i >= 0; i--)
	{
		Image * Heart;
		if (i < m_PlayerLife / 2)
		{
			Heart = IMGMANAGER->GetImage("PlayerLife");
			if (i == m_Beat)
			{
				Heart->Scale(70, 70);
			}
			else
			{
				Heart->Scale(50, 50);
			}
		}
		else if ((m_PlayerLife % 2) > 0 && i == m_PlayerLife / 2)
		{
			Heart = IMGMANAGER->GetImage("PlayerLifeHub");
			if (i == m_Beat)
			{
				Heart->Scale(70, 70);
			}
			else
			{
				Heart->Scale(50, 50);
			}
		}
		else
		{
			Heart = IMGMANAGER->GetImage("PlayerLifeEmpty");
			Heart->Scale(50, 50);
		}

		Heart->Position(((WINSIZEX - 250) - (i * 50)) + g_ptCam.x, (WINSIZEY - (50 * ((m_PlayerMaxLife / 5))) + g_ptCam.y));
		Heart->Render();
	}

}

void PlayerUI::CoinRender()
{
	Player * player = CHARTERMANAGER->GetPlayer();
	CurrentCoin = player->Getm_Coin();
	CurrentDiamond = player->Getm_Diamond();
	Image * XImg = IMGMANAGER->GetImage("XImg");
	Image * NumImg = IMGMANAGER->GetImage("DigitalNum");
	Image * CoinImg = IMGMANAGER->GetImage("PlayerCoin");
	Image * DiamondImg = IMGMANAGER->GetImage("PlayerDiamond");
	NumImg->Scale(30, 30);
	CoinImg->Color(0, 0, 0, 1.0f);
	DiamondImg->Color(0, 0, 0, 1.0f);
	XImg->Position((WINSIZEX - 150) + g_ptCam.x, (WINSIZEY - 50) + g_ptCam.y);
	XImg->Scale(10, 10);
	XImg->Render();
	XImg->Position((WINSIZEX - 150) + g_ptCam.x, (WINSIZEY - 90) + g_ptCam.y);
	XImg->Scale(10, 10);
	XImg->Render();

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
			NumImg->Position(((WINSIZEX - 150) + 30 * i) + g_ptCam.x, (WINSIZEY - 50) + g_ptCam.y);
			NumImg->GetRefCurrentFrameX() = Num[i];
		}
		else if (First == false && Num[i] != 0)
		{
			NumImg->Position(((WINSIZEX - 150) + 30 * i) + g_ptCam.x, (WINSIZEY - 50) + g_ptCam.y);
			NumImg->GetRefCurrentFrameX() = Num[i];
			First = true;
		}
		NumImg->Render();
	}

	Num[0] = CurrentDiamond / 1000;
	Num[1] = (CurrentDiamond - Num[0] * 1000) / 100;
	Num[2] = (CurrentDiamond - Num[0] * 1000 - Num[1] * 100) / 10;
	Num[3] = (CurrentDiamond - Num[0] * 1000 - Num[1] * 100 - Num[2] * 10) / 1;
	First = false;
	for (int i = 0; i < 4; i++)
	{
		if (First || i == 3)
		{
			NumImg->Position(((WINSIZEX - 150) + 30 * i) + g_ptCam.x, (WINSIZEY - 90) + g_ptCam.y);
			NumImg->GetRefCurrentFrameX() = Num[i];
		}
		else if (First == false && Num[i] != 0)
		{
			NumImg->Position(((WINSIZEX - 150) + 30 * i) + g_ptCam.x, (WINSIZEY - 90) + g_ptCam.y);
			NumImg->GetRefCurrentFrameX() = Num[i];
			First = true;
		}
		NumImg->Render();
	}

	CoinImg->Position((WINSIZEX - 200) + g_ptCam.x, (WINSIZEY - 50) + g_ptCam.y);
	CoinImg->Scale(50, 50);
	DiamondImg->Position((WINSIZEX - 200) + g_ptCam.x, (WINSIZEY - 90) + g_ptCam.y);
	DiamondImg->Scale(50, 50);
	CoinImg->Render();
	DiamondImg->Render();
}

void PlayerUI::InventoryOpen(int i)
{
	m_IsInven[i] = true;
}

void PlayerUI::InventoryRender()
{
	Player * player = CHARTERMANAGER->GetPlayer();
	Inventory * inven = player->Getm_inven();
	for (int i = 0; i < 5; i++)
	{
		Image * img;
		if (m_IsInven[i])
		{
			char imgkey[256];
			sprintf_s(imgkey, "hudslot%d", i + 1);
			img = IMGMANAGER->GetImage(imgkey);
			img->Scale(80, 80);
			img->Position((80 + (i * 100)) + g_ptCam.x, (WINSIZEY - 100) + g_ptCam.y);
			img->Render();
			if (inven[i].ItemKey != "")
			{
				ITEMMANAGER->FindItem(inven[i].ItemKey)->EffectRender();
				ITEMMANAGER->FindItem(inven[i].ItemKey)->Render();
			}
		}
	}
	if (m_IsPack)
	{
		Image * img2;
		img2 = IMGMANAGER->GetImage("hudslotaction2empty");
		img2->Scale(80, 120);
		img2->Position(80 + g_ptCam.x, (WINSIZEY - 210) + g_ptCam.y);
		img2->Render();

		if (inven[7].ItemKey != "")
		{
			ITEMMANAGER->FindItem(inven[7].ItemKey)->EffectRender();
			ITEMMANAGER->FindItem(inven[7].ItemKey)->Render();
		}
	}
	if (inven[6].ItemKey != "")
	{
		Image * img2;
		img2 = IMGMANAGER->GetImage("hudslot8");
		img2->Scale(80, 100);
		img2->Position(180 + g_ptCam.x, (WINSIZEY - 190) + g_ptCam.y);
		img2->Render();

		if (inven[6].ItemKey != "")
		{
			ITEMMANAGER->FindItem(inven[6].ItemKey)->EffectRender();
			ITEMMANAGER->FindItem(inven[6].ItemKey)->Render();
		}
	}
}

void PlayerUI::HpBeatOn()
{
	if (!m_IsPlayerDie)
	{
		m_Beat++;
		int i = m_PlayerLife / 2;
		bool j = ((m_PlayerLife % 2) != 0);

		m_Beat = m_Beat % (i + j);
	}
}



