#pragma once
#include "Singleton/MeoryPool.h"

enum TileProperty
{
	EMPTY,
	PASS,
	NONPASS
};

enum TileObject
{
	OBJECT_EMPTY,
	OBJECT_FLOOR,
	OBJECT_WALL,
	OBJECT_BROKENWALL,
	OBJECT_TWOBROKENWALL,
	OBJECT_DOOR,
	OBJECT_TRAP,
	OBJECT_SCAFFOLDING,
	OBJECT_TORCH,
	OBJECT_SHOPWALL,
	OBJECT_ENEMY,
	OBJECT_PLAYER,
	OBJECT_ROOM,
	OBJECT_ITEM,
	OBJECT_STAIRS,
	OBJECT_LOCKSTAIRS,
	OBJECT_LOCKDOOR,
	OBJECT_SHOPKEEPER,
	OBJECT_SHOPFLOOR,
	OBJECT_SHOPITEM,
	OBJECT_KEY,
	OBJECT_ITEMBOX,
	OBJECT_ETC
};

enum Mode
{
	MODE_DRAW,
	MODE_REMOVE
};

enum ColisionDir
{
	COLISION_LEFT,
	COLISION_RIGHT,
	COLISION_DOWN,
	COLISION_UP,
};

struct ST_TILE : public CMemeoryPool<ST_TILE, 100>
{
	//나눠지는 이미지에서 어떤 프레임을 가지고 있을지
	int		CurrentFrameX;
	int		CurrentFrameY;
	int     ObjFrameX;
	int     ObjFrameY;
	bool    m_IsPlayerSight;
	bool    m_IsPlayerLight;
	bool    m_IsPlayerFirstLight;
	//이미지가 여러장 필요한 경우 식별하기위한 변수
	float Xpos;
	float Ypos;
	string	ImageKey;
	string ImagKey2;
	string	ImageKey3;
	int		MaxFrameX;
	int		MaxFrameY;
	int		ObjMaxFrameX;
	int		ObjMaxFrameY;
	int BottomValue;
	TileProperty Property;
	TileObject Object;
	TileObject Object2;
	TileObject Object3;
};

class TileMap
{
private:
	//하나씩만 가지고 있을 정보값
	float PrevCamX;
	float PrevCamY;
	InstanceImage * m_TerianTex;
	bool m_IsBeate;
	bool m_IsFever;
	float DelayTime;
	MapInfo Info;
	//배열로 저장할 구조체
	ST_TILE ** m_Tiles;
	int StartIndexX;
	int StartIndexY;
	int EndIndexX;
	int EndIndexY;
	float AniTime;
	float m_PatternDelay = 0.0f;
	bool TileView;
	bool m_AllMapView;
	bool PassView;
	bool ObjectView;
	bool IndexView;
	bool Partern;
	int TilePatternNum;
public:
	TileMap();
	~TileMap();
	HRESULT init();
	void release();
	void update();
	void render();
	void BegineTileTexUpdate();
	void TexClear(UINT X, UINT Y, string TileName);
	void TexTileUpdate();
	void SetMaxTile(UINT X, UINT Y, float W, float H, string TileName);
	int AlphaMap(int x, int y, int Ray, int Sight);
	void TeriansetMap(int Xtile, int Ytile, LONG m_SelectFrameX, LONG m_SelectFramey, Mode m_Mode = MODE_DRAW, string imgKey1 = "", TileProperty Property = TileProperty::PASS, float Alpha = 1);
	void ObjectsetMap(int Xtile, int Ytile, LONG m_SelectFrameX, LONG m_SelectFramey, Mode m_Mode = MODE_DRAW, string imgKey1 = "", bool ObjectAdd = false, TileProperty Property = TileProperty::PASS, TileObject ObjectType = TileObject::OBJECT_EMPTY);
	void TerianAlphaUpdate();
	void LightOffMap(int StartX, int StartY, int EndX, int EndY);
	bool LoadMap(LPCSTR SaveName, string FloorKey);
	bool SaveMap(LPCSTR SaveName);
	void TerranMapBeat(int x, int y, float DelayTime);
	void SetMonsterProperty(int Xtile, int Ytile, TileObject ObjectType, TileProperty Property, string ImageKey);
	ST_TILE * GetTile(UINT IndexX, UINT IndexY);
	void Sort();
	bool ColiisionTile(D3DXVECTOR2 Pos, D3DXVECTOR2 Scale, ColisionDir Dir);
	void AllViewMap();
	MapInfo GetInfo() { return Info; }
	MAKEGETSET(bool, PassView);
	MAKEGETSET(bool, ObjectView);
	MAKEGETSET(bool, IndexView);
	MAKEGETSET(bool, TileView);
	MAKEGETSET(bool, m_IsBeate);
	MAKEGETSET(bool, m_IsFever);
	MAKEGETSET(bool, m_AllMapView)
};


