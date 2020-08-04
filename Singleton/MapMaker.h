#pragma once
#include "SingletonBase.h"
#include "BspTree/BspTreeNode.h"

#define MAXX 5
#define MAXY 10

class MapMaker : public SingletonBase<MapMaker>
{
private:

	vector<AREASTATE> m_Area;
	map<int, OBJECTSTATE> objdata;
	vector<Room *> Rooms;
	int m_width;	//∏  ∞°∑Œ≈©±‚
	int m_height; //∏  ºº∑Œ≈©±‚
	int m_DivNum;
	int m_Com;
	float m_DivRange;
	float m_MidSize;
	float m_SizeRange;
	BspTreeNode m_Root;
	bool m_isCreated = false;

public:
	void Division(queue<BspTreeNode*>& Node, RandomGet& Random);
	void RoomMaker();
	void Coalescence();
	void FillData();
	AREASTATE GetData(int x, int y) const { return m_Area[x + y * m_width]; }
	OBJECTSTATE GetObjData(int x, int y) const 
	{ 
		if (objdata.find(x + y * m_width) != objdata.end())
		{
			return objdata.find(x + y * m_width)->second;
		}

		return OBJECTSTATE::OBJECT_NO;
	}
	queue<BspTreeNode*> GetRootNode();
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	void SetWidth(int Width);
	void SetHeight(int Height);
	void MapCreatInit(int Width, int Height, int DivNum, float DivRange, float MidSize, float SizeRange, int Com);
	void CreateMap();
	int ObjSize();
	MapMaker();
	MapMaker(int Width, int Height, int DivNum, float DivRange, float MidSize, float SizeRange, int Com);
	~MapMaker();

	MAKEGETSET(int, m_DivNum);
	MAKEGETSET(int, m_Com);
	MAKEGETSET(float, m_DivRange);
	MAKEGETSET(float, m_MidSize);
	MAKEGETSET(float, m_SizeRange);
	MAKEGETSET(vector<AREASTATE>, m_Area);
	MAKEGET(vector<Room *>, Rooms);
};
