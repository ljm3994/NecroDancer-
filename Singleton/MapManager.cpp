#include "stdafx.h"
#include "MapManager.h"
#include "Map/Shop.h"

MapManager::MapManager()
{
	m_TileMap = new TileMap();
}


MapManager::~MapManager()
{
	m_TileMap->release();
	SAFE_DELETE(m_TileMap);
}

void MapManager::MapInit(int Width, int Height, int TileSizeX, int TileSizeY, string TileName)
{
	m_Width = Width;
	m_Height = Height;
	this->TileSizeX = TileSizeX;
	this->TileSizeY = TileSizeY;
	isPlayerPos = false;
	if (m_Shop)
	{
		SAFE_DELETE(m_Shop);
	}
	m_Shop = new Shop();
	m_TileMap->SetMaxTile(Width, Height, static_cast<float>(TileSizeX), static_cast<float>(TileSizeY), TileName);
}

void MapManager::TexMapUpdate()
{
	m_TileMap->TexTileUpdate();
}

void MapManager::MapSetting()
{
	int CLevel = GAMEMANAGER->Getm_CurrentLevel();
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			AREASTATE stat;
			int CurrentX;
			stat = MAPMAKERMANAGER->GetData(i, j);
			switch (stat)
			{
			case AREASTATE::AREA_DOOR:
				m_TileMap->ObjectsetMap(i, j, 0, 0, MODE_DRAW, "Door", false, TileProperty::NONPASS, TileObject::OBJECT_DOOR);
				break;
			case AREASTATE::AREA_SIDEDOOR:
				m_TileMap->ObjectsetMap(i, j, 0, 0, MODE_DRAW, "DoorSide", false, TileProperty::NONPASS, TileObject::OBJECT_DOOR);
				break;
			case AREASTATE::AREA_WALL:
				WallCreate(i, j, CLevel);
				break;
			case AREASTATE::AREA_NOBROKENNWALL:
				m_TileMap->ObjectsetMap(i, j, 0, 0, MODE_DRAW, "UnbrokenWall", false, TileProperty::NONPASS, TileObject::OBJECT_WALL);
				break;
			case AREASTATE::AREA_TORCH:
				char ImageKey[256];
				sprintf_s(ImageKey, "Wall%d", CLevel % 5);
				m_TileMap->ObjectsetMap(i, j, 0, 0, MODE_DRAW, ImageKey, false, TileProperty::NONPASS, TileObject::OBJECT_WALL);
				m_TileMap->ObjectsetMap(i, j, 0, 0, MODE_DRAW, "Torch", true, TileProperty::NONPASS, TileObject::OBJECT_TORCH);
				break;
			case AREASTATE::AREA_SHOPWALL:
				CurrentX = GetInt(4);
				m_TileMap->ObjectsetMap(i, j, CurrentX, 0, MODE_DRAW, "ShopWall", false, TileProperty::NONPASS, OBJECT_SHOPWALL);
				break;
			}
		}
	}
}

void MapManager::MapUpdate()
{
	m_TileMap->update();
}

void MapManager::MapRender()
{
	m_TileMap->render();
}

void MapManager::MapClear(int Width, int Height, string TileName)
{
	m_TileMap->TexClear(Width, Height, TileName);
}

void MapManager::MapLightOff(int X, int Y, int EndX, int EndY)
{
	m_TileMap->LightOffMap(X, Y, EndX, EndY);
}

void MapManager::AllView()
{
	m_TileMap->AllViewMap();
}

void MapManager::BeginTileTexUpdate()
{
	m_TileMap->BegineTileTexUpdate();
}

void MapManager::LoadMap(LPCSTR MapName, string FloorKey)
{
	m_TileMap->LoadMap(MapName, FloorKey);
}

int MapManager::PlayerRayUpdate(int x, int y, int Ray, int Sight)
{
	return m_TileMap->AlphaMap(x, y, Ray, Sight);
}

void MapManager::WallCreate(int x, int y, int Level)
{
	
	char ImageKey[256];
	sprintf_s(ImageKey, "Wall%d", Level % 5);
	int CurrentX = GetInt(4);
	m_TileMap->ObjectsetMap(x, y, CurrentX, 0, MODE_DRAW, ImageKey, false, TileProperty::NONPASS, TileObject::OBJECT_BROKENWALL);
}

void MapManager::SetBeat(bool Beat)
{
	m_TileMap->Setm_IsBeate(Beat);
}

bool MapManager::GetFever()
{
	return m_TileMap->Getm_IsFever();
}

void MapManager::CrateStair()
{
	vector<Room *> Rooms = MAPMAKERMANAGER->GetRooms();
	while (1)
	{
		int Index = GetInt(Rooms.size() - 1);
		Room * SelectRoom = Rooms[Index];
		if (SelectRoom->m_Property != ROOM_PLAYER && SelectRoom->m_Property != ROOM_SHOP)
		{
			SelectRoom->m_Property = ROOM_EXIT;
			D3DXVECTOR2 m_Pos;
			m_Pos.x = static_cast<float>(SelectRoom->m_X + (SelectRoom->m_Width / 2));
			m_Pos.y = static_cast<float>(SelectRoom->m_Y + (SelectRoom->m_Height / 2));
			ST_TILE * CurrentTile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
			CurrentTile->Object = OBJECT_LOCKSTAIRS;
			CurrentTile->ImageKey = "LockStairs";
			CurrentTile->Property = NONPASS;
			ExitPos = m_Pos;
			ExitPos.x += 1;
			break;
		}
	}
}

void MapManager::CreateKey()
{
	vector<Room *> Rooms = MAPMAKERMANAGER->GetRooms();
	while (1)
	{
		int Index = GetInt(Rooms.size() - 1);
		Room * SelectRoom = Rooms[Index];
		if (SelectRoom->m_Property != ROOM_PLAYER && SelectRoom->m_Property != ROOM_EXIT && SelectRoom->m_Property != ROOM_SHOP)
		{
			D3DXVECTOR2 m_Pos;
			m_Pos.x = static_cast<float>(SelectRoom->m_X + (SelectRoom->m_Width / 2));
			m_Pos.y = static_cast<float>(SelectRoom->m_Y + (SelectRoom->m_Height / 2));
			ST_TILE * CurrentTile = MAPMANAGER->GetAllMap()->GetTile(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y));
			CurrentTile->Object2 = OBJECT_KEY;
			CurrentTile->ImagKey2 = "LockKey";
			CurrentTile->Property = PASS;
			KeyPos = m_Pos;
			KeyPos.x += 1;
			break;
		}
	}
}

void MapManager::SetFever(bool Fever)
{
	m_TileMap->Setm_IsFever(Fever);
}

void MapManager::DeleteMap(int i, int j)
{
	m_TileMap->ObjectsetMap(i, j, 0, 0, MODE_REMOVE);
}

