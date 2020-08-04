#include "stdafx.h"
#include "Shop.h"


Shop::Shop()
{
}


Shop::~Shop()
{
}

void Shop::PlayShopSound()
{
}

void Shop::SellItem(ItemBase * Item, Player * pl)
{
	int Price = Item->Getm_Price();
	pl->Setm_Coin(pl->Getm_Coin() + Price);
}

bool Shop::BuyItem(string strKey, Player * pl)
{
	int Price = ITEMMANAGER->FindItem(strKey)->Getm_Price();
	Inventory * in = pl->Getm_inven();
	if (pl->Getm_Coin() >= Price)
	{
		if (ITEMMANAGER->FindItem(strKey)->GetItemKind() == INVEN_HEART)
		{
			pl->Setm_Coin(pl->Getm_Coin() - Price);
			ITEMMANAGER->FindItem(strKey)->GetItem(pl);
			return true;
		}
		else if (in[ITEMMANAGER->FindItem(strKey)->GetItemKind() - 1].ItemKey != strKey)
		{
			if (ITEMMANAGER->FindItem(strKey)->GetItem(pl))
			{
				ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x), static_cast<UINT>(pl->Getm_Pos().y));
				if (tile->Object2 == OBJECT_ITEM)
				{
					SellItem(ITEMMANAGER->FindItem(tile->ImagKey2), pl);
					tile->Object2 = OBJECT_EMPTY;
					tile->ImagKey2 = "";
				}
				pl->SetInvenTory(ITEMMANAGER->FindItem(strKey)->GetItemKind() - 1, strKey);
				pl->Setm_Coin(pl->Getm_Coin() - Price);
				return true;
			}
			return false;
		}
	}
	return false;
}

void Shop::Render(float Alpha)
{
	Image * img = IMGMANAGER->GetImage("ShopKeeper");
	img->Scale(70, 100);
	img->Color(0, 0, 0, Alpha);
	img->Position(m_ShopKeeperPos.x * 50, (m_ShopKeeperPos.y * 50) + 25);
	img->PlayAnimation(PlayState::ANI_LOOP, true, 0.2f);
}

void Shop::PriceRender(D3DXVECTOR2 pos, string srtKey, float Alpha)
{
	Image * img = IMGMANAGER->GetImage("PlayerCoin");
	img->Scale(10, 10);
	img->Color(0, 0, 0, Alpha);
	img->Position((pos.x * 50) + 25, (pos.y * 50) - 25);
	img->Render();

	Image * NumImg = IMGMANAGER->GetImage("DigitalNum2");
	NumImg->Scale(13, 13);
	int CurrentCoin = ITEMMANAGER->FindItem(srtKey)->Getm_Price();
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
			NumImg->Position((((pos.x * 50)- 25) + 13 * i), (pos.y * 50) - 25);
			NumImg->GetRefCurrentFrameX() = Num[i];
			NumImg->Render();
		}
		else if (First == false && Num[i] != 0)
		{
			NumImg->Position((((pos.x * 50) - 25) + 13 * i), (pos.y * 50) - 25);
			NumImg->GetRefCurrentFrameX() = Num[i];
			First = true;
			NumImg->Render();
		}

	}

}

void Shop::ShopSfxVolum()
{
	D3DXVECTOR2 Pos = CHARTERMANAGER->GetPlayer()->Getm_Pos();

	float Distance = GetDistance(Pos.x, Pos.y, m_ShopKeeperPos.x, m_ShopKeeperPos.y);
	m_Volum = GAMEMANAGER->GetsfxVolum() - (Distance / 10);

	if (m_Volum <= 0)
	{
		m_Volum = 0;
	}

}

void Shop::Update()
{
	ShopSfxVolum();
	SOUNDMANAGER->SoundVolumChange("ShopKeeper", m_Volum);
}

void Shop::Init()
{
	m_Volum = 0.0f;
	CreateShop();
}

void Shop::CreateShop()
{
	if (GAMEMANAGER->Getm_CurrentLevel() == -1)
	{
		Xpos = 11;
		Ypos = 31;
		m_Width = 5;
		m_Height = 8;
	}
	else
	{
		vector<Room *> vecRoom = MAPMAKERMANAGER->GetRooms();

		while (1)
		{
			random_device rd;
			RandomGet Random(rd());
			uniform_int_distribution<int> Ran(0, vecRoom.size() - 1);
			int ShopRandom = Ran(Random);
			vecRoom[ShopRandom]->m_Property = ROOM_SHOP;
			if (vecRoom[ShopRandom]->m_Width >= 9 && vecRoom[ShopRandom]->m_Height >= 9 && vecRoom[ShopRandom]->m_Property != ROOM_PLAYER)
			{
				for (int y = vecRoom[ShopRandom]->m_Y; y < vecRoom[ShopRandom]->getBottom() + 1; y++)
				{
					for (int x = vecRoom[ShopRandom]->m_X; x < vecRoom[ShopRandom]->getRight() + 1; x++)
					{
						if (vecRoom[ShopRandom]->isWallPostion(x, y, 0, 0, vecRoom[ShopRandom]->GetAllRooms()))
						{
							int CurrentX = GetInt(4);
							MAPMANAGER->GetAllMap()->ObjectsetMap(x, y, CurrentX, 0, MODE_DRAW, "ShopWall", false, TileProperty::NONPASS, OBJECT_SHOPWALL);
						}
					}
				}

				for (auto& door : vecRoom[ShopRandom]->m_Doors)
				{
					MAPMANAGER->GetAllMap()->ObjectsetMap(door.x, door.y, 0, 0, MODE_DRAW, "", false, TileProperty::PASS, OBJECT_EMPTY);
				}
				Xpos = vecRoom[ShopRandom]->m_X;
				Ypos = vecRoom[ShopRandom]->m_Y;
				m_Width = vecRoom[ShopRandom]->m_Width;
				m_Height = vecRoom[ShopRandom]->m_Height;
				break;							
			}
		}
	}
	ItemCreate();
}

void Shop::ItemCreate()
{
	SOUNDMANAGER->play("ShopKeeper", m_Volum);
	m_ShopKeeperPos = D3DXVECTOR2(static_cast<float>(Xpos + (m_Width / 2)), static_cast<float>(Ypos + (m_Height / 2)));
	MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_ShopKeeperPos.x), static_cast<int>(m_ShopKeeperPos.y), OBJECT_SHOPKEEPER, NONPASS, "ShopKeeper");
	MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_ShopKeeperPos.x - 1), static_cast<int>(m_ShopKeeperPos.y), 0, 0, MODE_DRAW, "ShopFloor", false, PASS, OBJECT_SHOPFLOOR);
	MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_ShopKeeperPos.x + 1), static_cast<int>(m_ShopKeeperPos.y), 0, 0, MODE_DRAW, "ShopFloor", false, PASS, OBJECT_SHOPFLOOR);
	random_device rd;
	RandomGet Random(rd());
	uniform_int_distribution<int> Ran(0, ITEMMANAGER->GetMaxSize() - 1);
	int ShopRandom = Ran(Random);
	vector<int> SelectNum;
	string ItemName = "";
	if (CHARTERMANAGER->Getm_Player()->Getm_inven()[7].ItemKey == "")
	{
		ItemName = "BackPack";
		SelectNum.emplace_back(14);
	}
	else
	{
		while (1)
		{
			uniform_int_distribution<int> Ran(0, ITEMMANAGER->GetMaxSize() - 1);
			int ShopRandom = Ran(Random);
			ItemName = ITEMMANAGER->FindItem(ShopRandom);
			if (ItemName.find("Coin") != string::npos)
			{
				continue;
			}
			SelectNum.emplace_back(ShopRandom);
			break;
		}
	}
	MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_ShopKeeperPos.x - 1), static_cast<int>(m_ShopKeeperPos.y - 2), 0, 0, MODE_DRAW, ItemName, true, NONPASS, OBJECT_SHOPITEM);
	while (1)
	{
		uniform_int_distribution<int> Ran(0, ITEMMANAGER->GetMaxSize() - 1);
		int ShopRandom = Ran(Random);
		if (SelectNum[0] == ShopRandom)
		{
			continue;
		}
		ItemName = ITEMMANAGER->FindItem(ShopRandom);
		if (ItemName.find("Coin") != string::npos)
		{
			continue;
		}
		SelectNum.emplace_back(ShopRandom);
		break;
	}
	MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_ShopKeeperPos.x), static_cast<int>(m_ShopKeeperPos.y - 2), 0, 0, MODE_DRAW, ItemName, true, NONPASS, OBJECT_SHOPITEM);
	while (1)
	{
		uniform_int_distribution<int> Ran(0, ITEMMANAGER->GetMaxSize() - 1);
		int ShopRandom = Ran(Random);
		if (SelectNum[0] == ShopRandom || SelectNum[1] == ShopRandom)
		{
			continue;
		}
		ItemName = ITEMMANAGER->FindItem(ShopRandom);
		if (ItemName.find("Coin") != string::npos)
		{
			continue;
		}
		SelectNum.emplace_back(ShopRandom);
		break;
	}
	MAPMANAGER->GetAllMap()->ObjectsetMap(static_cast<int>(m_ShopKeeperPos.x + 1), static_cast<int>(m_ShopKeeperPos.y - 2), 0, 0, MODE_DRAW, ItemName, true, NONPASS, OBJECT_SHOPITEM);
}
