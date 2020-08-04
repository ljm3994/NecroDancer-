#pragma once
#include "BspTreeHelper.h"
#define MINIMUMSIZEX 10
#define MINIMUMSIZEROOM 5

class BspTreeNode
{
	RoomData m_RoomInfo;
	Room m_Room;
	unique_ptr<BspTreeNode> m_LeftChild;
	unique_ptr<BspTreeNode> m_RightChild;
	vector<Point> m_Way;
	bool m_IsWidth;
	void GetSideRoom(DIRECTION dir, vector<Room*>& rooms); // 해당 방향에 있는 방을 가져온다
	void GetAllRoom(vector<RoomData>& rooms); // 모든 방을 가져온다
	void GetAllWay(vector<Point>& ways);  // 모든 통로를 가져온다
	Point getDoorNextPos(const Point& door, const Room& room);  //해당 문의 다음 위치를 가져온다.
	bool isConnect(Point startPos, Point EndPos, const vector<Point>& ways, vector<Point>& visite); // 방이 서로 연결되어있는가
	bool isContain(const vector<RoomData>& rooms, const Point & m_pos);     
	bool isContain(const vector<RoomData>& rooms, int x, int y);
	bool isContain(const vector<Point>& rooms, const Point & m_pos);
	bool isContain(const vector<Point>& rooms, int x, int y);
	bool isHallPos(const Point & pos, DIRECTION dir, RoomData area, const vector<RoomData>& rooms, const vector<Point>& Other, const vector<Point>& visite);
	bool WidthDivision(float Range, RandomGet& Random);
	bool HeightDivision(float Range, RandomGet& Random);
	void Connect(int Com, const vector<Room*>& leftChild, const vector<Room*>& RightChild, RandomGet& Random);
	bool MakeWay(Point StartPos, Point EndPos, const RoomData & area, int Com, const vector<RoomData>& Rooms, vector<Point>& visite, vector<Point>& OtherWay, RandomGet& Random);
	Point GetRandomDoor(const Room & room, bool isBegin, RandomGet& Random);
public:
	BspTreeNode * GetRightChile();
	BspTreeNode * GetLeftChile();
	bool IsHasChile();
	void FillData(int Width, int Height, vector<AREASTATE>& Data, vector<Room *>& Rooms);
	void FillObjectData(int Width, int Height, const vector<AREASTATE>& Data, map<int, OBJECTSTATE>& objdata);
	void Coalescence(int Com, RandomGet& Random);  // 방연결 함수
	void RoomMake(float MidSize, float Range, RandomGet& Random);   // 방을 생성한다.
	void Division(float Range, RandomGet& Random);     // 정해진 범위로 분할 한다
	void Clear(int x, int y, int width, int height);
	BspTreeNode() {};
	BspTreeNode(int x, int y, int width, int height);

};

