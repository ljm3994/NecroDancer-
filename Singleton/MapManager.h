#pragma once
#include "SingletonBase.h"
#include "Map/TileMap.h"

class Shop;

class MapManager : public SingletonBase<MapManager>
{
	TileMap * m_TileMap;
	Shop * m_Shop;
	int m_Width;
	int m_Height;
	int TileSizeX;
	int TileSizeY;
	D3DXVECTOR2 PlayerPos;
	D3DXVECTOR2 ExitPos;
	D3DXVECTOR2 KeyPos;
	bool isPlayerPos;
public:
	MapManager();
	~MapManager();
	void MapInit(int Width, int Height, int TileSizeX, int TileSizeY, string TileName);
	void TexMapUpdate();
	void MapSetting();
	void MapUpdate();
	void MapRender();
	void MapClear(int Width, int Height, string TileName);
	void MapLightOff(int X, int Y, int EndX, int EndY);
	void AllView();
	void BeginTileTexUpdate();
	void LoadMap(LPCSTR MapName, string FloorKey);
	int PlayerRayUpdate(int x, int y, int Ray, int Sight);
	void WallCreate(int x, int y, int Level);
	void SetBeat(bool Beat);
	bool GetFever();
	void CrateStair();
	void CreateKey();
	void SetFever(bool Fever);
	void DeleteMap(int i, int j);
	ST_TILE * GetMap(int i, int j) { return m_TileMap->GetTile(i, j); }
	TileMap * GetAllMap() { return m_TileMap; }
	MAKEGETSET(Shop *, m_Shop);
	MAKEGET(D3DXVECTOR2, ExitPos);
	MAKEGET(D3DXVECTOR2, KeyPos);
	MAKEGET(int, m_Width);
	MAKEGET(int, m_Height);
};

