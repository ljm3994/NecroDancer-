#include "stdafx.h"
#include "MapInfo.h"


MapInfo::MapInfo(string Name, UINT TileMaxX, UINT TileMaxY, float TileWidth, float TileHeight)
	:MapName(Name), TileMaxX(TileMaxX), TileMaxY(TileMaxY), TileWidth(TileWidth), TileHeight(TileHeight)
{
}

MapInfo::~MapInfo()
{
}

void MapInfo::SetMaxTile(UINT TileMaxX, UINT TileMaxY)
{
	this->TileMaxX = TileMaxX;
	this->TileMaxY = TileMaxY;
}

void MapInfo::SetSizeTile(float TileWidth, float TileHeight)
{
	this->TileWidth = TileWidth;
	this->TileHeight = TileHeight;
}
