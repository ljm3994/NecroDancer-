#pragma once

static bool ShopIs = false;
static bool OnlyShop = false;

struct Point
{
	int x;
	int y;
};

enum AREASTATE
{
	AREA_EMPTY,
	AREA_ROOM,
	AREA_WALL,
	AREA_NOBROKENNWALL,
	AREA_DOOR,
	AREA_SIDEDOOR,
	AREA_LOAD,
	AREA_TORCH,
	AREA_SHOP,
	AREA_SHOPWALL
};
enum ROOMPROPERTY
{
	ROOM_ENEMY,
	ROOM_SHOP,
	ROOM_PLAYER,
	ROOM_EXIT,
	ROOM_ETC
};
enum OBJECTSTATE
{
	OBJECT_NO,
	OBJECT_BOX,
	OBJECT_DOWNTRAP,
	OBJECT_MOVETRAP,
	OBJECT_MONSTER
};
enum DIRECTION {
	DIR_TOP,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_BOTTOM
};


struct RoomData
{
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

	RoomData() : m_X(0), m_Y(0), m_Width(0), m_Height(0) { }
	RoomData(int x, int y, int width, int height) : m_X(x), m_Y(y), m_Width(width), m_Height(height) { }

	bool isConnect(const RoomData& other) const;
	bool isContain(const Point& pos) const;

	int getRight() const { return m_X + m_Width - 1; }
	int getBottom() const { return m_Y + m_Height - 1; }
};

struct Room : RoomData
{
	vector<Room*> GetAllRooms();
	ROOMPROPERTY m_Property;
	vector<Room*> m_ConnectedRooms;
	vector<Point> m_Doors;
	Point m_ObjectBox;
	Point m_Trap;
	Room() : RoomData(), m_IsVisited(false) { }
	Room(int x, int y, int width, int height) : RoomData(x, y, width, height), m_IsVisited(false) { }

	void FillData(int width, int height, vector<AREASTATE>& data, vector<Room *>& Rooms);
	void FiilObjectData(int width, int height, const vector<AREASTATE>& data, map<int, OBJECTSTATE> &obdata);
	bool isWallPostion(int x, int y, int width, int height, const vector<Room*>& rooms);

	bool m_IsVisited;
};
