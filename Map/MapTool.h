#pragma once
#include "GameNode.h"

//이미지타일(인게임)
#define TILESIZEGAME 48
#define MaxImgKey 13

struct ImageInfo
{
	string ImageName;
	int MaxFrameX;
	int MaxFrameY;
	float FrameSizeX;
	float FrameSizey;
	float ImageWitdh;
	float ImageHegiht;
};

class MapTool : public GameNode
{
	TileMap * Tile;
	vector<ImageInfo> m_ImageInfo;
	Mode m_Mode;
	string ImageKey1;
	string ImageKey2;
	TileProperty _SelectType;
	TileObject m_Object;
	bool m_ObjectSelect;
	bool m_bStateSelect[2];
	bool TileSettingOpen;
	UINT m_SelectFramx;
	UINT m_SelectFramy;
	bool m_PassView;
	bool ObectView;
	bool m_TileIndexView;
	string MapName;
	int SelectImage;
	int X, Y;
	float W, H;
public:
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void ImgSet();
	//맵툴세팅
	//맵세팅
	void setMap();
	//세이브
	//로드
	void ImGuiSetting();
	void ImGuiMain();
	void ImGuiTileSettingMenu(bool * Popen);
	MapTool();
	~MapTool() {}
	MAKEGET(TileMap *, Tile);
};

