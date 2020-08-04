#include "stdafx.h"
#include "Coin.h"


Coin::Coin()
{
}


Coin::~Coin()
{
}

HRESULT Coin::Init()
{
	return S_OK;
}

void Coin::Update()
{
}

void Coin::Render()
{
}

int Coin::GetItemDmg()
{
	return m_CoinCount;
}

void Coin::PutItem(D3DXVECTOR2 pos, float Alpha)
{

}

void Coin::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	char coinname[256];
	sprintf_s(coinname, "Coin%d", m_CoinCount);
	Image * img = IMGMANAGER->GetImage(coinname);
	img->Color(0, 0, 0, m_Alpha);
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(30, 30);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = 0;
	img->Render();
}
void Coin::EffectRender()
{

}
bool Coin::GetItem(Player * player)
{
	int Fever = 1;

	if (MAPMANAGER->GetFever())
	{
		Fever = 2;
	}
	if (player->Getm_Coin() <= 9999)
	{
		SOUNDMANAGER->play("CoinPickUp", GAMEMANAGER->GetsfxVolum());
		if (m_CoinCount < 10)
		{
			int CoinNum = player->Getm_Coin();
			player->Setm_Coin(CoinNum + (m_CoinCount * Fever));
		}
		else
		{
			double per[5] = { 20, 40, 20, 15, 5 };
			int count = GetRandom(per, 5);
			int CoinNum = player->Getm_Coin();
			switch (count)
			{
			case 0:
				player->Setm_Coin(CoinNum + (m_CoinCount * Fever));
				break;
			case 1:
				player->Setm_Coin(CoinNum + (GetFromIntTo(20, 30)* Fever));
				break;
			case 2:
				player->Setm_Coin(CoinNum + (GetFromIntTo(30, 40)* Fever));
				break;
			case 3:
				player->Setm_Coin(CoinNum + (GetFromIntTo(40, 55)* Fever));
				break;
			case 4:
				player->Setm_Coin(CoinNum + (GetFromIntTo(60, 100)* Fever));
				break;
			}
		}

		ST_TILE * TileTemp = MAPMANAGER->GetMap(static_cast<int>(player->Getm_Pos().x), static_cast<int>(player->Getm_Pos().y));
		TileTemp->Object2 = TileObject::OBJECT_EMPTY;
		TileTemp->ImagKey2 = "";
		return true;
	}
	return false;
}
