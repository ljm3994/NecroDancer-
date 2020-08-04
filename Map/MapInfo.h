#pragma once
class MapInfo
{
	string MapName;
	UINT TileMaxX;
	UINT TileMaxY;
	float TileWidth;
	float TileHeight;
	D3DXVECTOR2	m_ZeroStartPos;
public:
	MapInfo(string Name, UINT TileMaxX, UINT TileMaxY, float TileWidth, float TileHeight);
	MapInfo() {}
	~MapInfo();
	void SetMaxTile(UINT TileMaxX, UINT TileMaxY);
	void SetSizeTile(float TileWidth, float TileHeight);
	MAKEGET(UINT, TileMaxX);
	MAKEGET(UINT, TileMaxY);
	MAKEGET(float, TileWidth);
	MAKEGET(float, TileHeight);
	MAKEGETSET(D3DXVECTOR2, m_ZeroStartPos);
};

