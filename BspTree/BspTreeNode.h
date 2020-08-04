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
	void GetSideRoom(DIRECTION dir, vector<Room*>& rooms); // �ش� ���⿡ �ִ� ���� �����´�
	void GetAllRoom(vector<RoomData>& rooms); // ��� ���� �����´�
	void GetAllWay(vector<Point>& ways);  // ��� ��θ� �����´�
	Point getDoorNextPos(const Point& door, const Room& room);  //�ش� ���� ���� ��ġ�� �����´�.
	bool isConnect(Point startPos, Point EndPos, const vector<Point>& ways, vector<Point>& visite); // ���� ���� ����Ǿ��ִ°�
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
	void Coalescence(int Com, RandomGet& Random);  // �濬�� �Լ�
	void RoomMake(float MidSize, float Range, RandomGet& Random);   // ���� �����Ѵ�.
	void Division(float Range, RandomGet& Random);     // ������ ������ ���� �Ѵ�
	void Clear(int x, int y, int width, int height);
	BspTreeNode() {};
	BspTreeNode(int x, int y, int width, int height);

};

