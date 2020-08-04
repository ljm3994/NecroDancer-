#include "stdafx.h"
#include "BspTreeNode.h"

// 지정한 방향 주변의 모든 방을 담아주는 함수
void BspTreeNode::GetSideRoom(DIRECTION dir, vector<Room*>& rooms)
{
	if (!IsHasChile())
	{
		rooms.emplace_back(&m_Room);
		return;
	}

	switch (dir)
	{
	case DIR_TOP:
		if (m_LeftChild != nullptr)
			m_LeftChild->GetSideRoom(dir, rooms);
		if (m_RightChild != nullptr && m_IsWidth)
			m_RightChild->GetSideRoom(dir, rooms);
		break;
	case DIR_RIGHT:
		if (m_LeftChild != nullptr && !m_IsWidth)
			m_LeftChild->GetSideRoom(dir, rooms);
		if (m_RightChild != nullptr)
			m_RightChild->GetSideRoom(dir, rooms);
		break;
	case DIR_LEFT:
		if (m_LeftChild != nullptr)
			m_LeftChild->GetSideRoom(dir, rooms);
		if (m_RightChild != nullptr && !m_IsWidth)
			m_RightChild->GetSideRoom(dir, rooms);
		break;
	case DIR_BOTTOM:
		if (m_LeftChild != nullptr && m_IsWidth)
			m_LeftChild->GetSideRoom(dir, rooms);
		if (m_RightChild != nullptr)
			m_RightChild->GetSideRoom(dir, rooms);
		break;
	}
}

// 모든 방을 가져오는 함수
void BspTreeNode::GetAllRoom(vector<RoomData>& rooms)
{
	if (!IsHasChile())
	{
		rooms.emplace_back(RoomData(m_Room.m_X, m_Room.m_Y, m_Room.m_Width, m_Room.m_Height));
	}

	if (m_LeftChild != nullptr)
		m_LeftChild->GetAllRoom(rooms);
	if (m_RightChild != nullptr)
		m_RightChild->GetAllRoom(rooms);
}

// 모든 통로를 가져오는 함수
void BspTreeNode::GetAllWay(vector<Point>& ways)
{
	if (m_LeftChild != nullptr)
		m_LeftChild->GetAllWay(ways);
	if (m_RightChild != nullptr)
		m_RightChild->GetAllWay(ways);

	for (auto& w : m_Way)
	{
		ways.emplace_back(w);
	}
}

// 현재 문 위치에서 다음 문의 위치를 반환해준다
Point BspTreeNode::getDoorNextPos(const Point & door, const Room & room)
{
	int dx, dy;
	Point nextDoor;

	if (door.y == room.m_Y)
	{
		dx = 0;
		dy = -1;
	}
	else if (door.x == room.getRight())
	{
		dx = 1;
		dy = 0;
	}
	else if (door.y == room.getBottom())
	{
		dx = 0;
		dy = 1;
	}
	else
	{
		dx = -1;
		dy = 0;
	}

	nextDoor.x = door.x + dx;
	nextDoor.y = door.y + dy;
	return nextDoor;
}

// 현재 연결되어는지 확인 하는 함수
bool BspTreeNode::isConnect(Point startPos, Point EndPos, const vector<Point>& ways, vector<Point>& visite)
{
	if (startPos.x == EndPos.x && startPos.y == EndPos.y)
		return true;

	visite.emplace_back(startPos);

	vector<Point> Pos;
	Pos.emplace_back(Point{ startPos.x - 1, startPos.y });
	Pos.emplace_back(Point{ startPos.x, startPos.y - 1 });
	Pos.emplace_back(Point{ startPos.x + 1, startPos.y });
	Pos.emplace_back(Point{ startPos.x, startPos.y + 1 });

	for (auto& p : Pos)
	{
		if (!isContain(visite, p) && isContain(ways, p))
		{
			if (isConnect(p, EndPos, ways, visite))
			{
				return true;
			}
		}
	}

	return false;
}

// 현재 저장되어있는 데이터에 지정한 값이 들어있는지 확인 하는 함수
bool BspTreeNode::isContain(const vector<RoomData>& rooms, const Point & m_pos)
{
	return any_of(rooms.begin(), rooms.end(), [&](const RoomData& r)
		{
			return r.isContain(m_pos);
		});
}

bool BspTreeNode::isContain(const vector<RoomData>& rooms, int x, int y)
{
	return isContain(rooms, Point{ x, y });
}

bool BspTreeNode::isContain(const vector<Point>& rooms, const Point & m_pos)
{
	return any_of(rooms.begin(), rooms.end(), [&](const Point& r)
		{
			return r.x == m_pos.x && r.y == m_pos.y;
		});
}
bool BspTreeNode::isContain(const vector<Point>& rooms, int x, int y)
{
	return isContain(rooms, Point{ x, y });
}

// 복도의 위치를 가져오는 함수(해당 함수 수정으로 복도의 두께도 변경할 수 있다)
bool BspTreeNode::isHallPos(const Point & pos, DIRECTION dir, RoomData area, const vector<RoomData>& rooms, const vector<Point>& Other, const vector<Point>& visite)
{
	int dx, dy;

	// 양 쪽에 통로가 이미 존재하는 확인 할 수 있다.
	// 1로 설정하는 이유는 현재 통로의 두께는 1이기 때문이다.
	switch (dir)
	{
	case DIR_TOP:
	case DIR_BOTTOM:
		dx = 1;
		dy = 0;
		break;
	case DIR_LEFT:
	case DIR_RIGHT:
		dx = 0;
		dy = 1;
		break;
	}

	//방과 겹치거나 방문했던 곳 또 방문하면 안 되므로 이 두경우를 먼저 체크해서 제외한다.
	if (isContain(rooms, pos) || isContain(visite, pos))
		return false;

	//이미 존재하는 복도 위에 있는 경우 진행한다(해당 복도와 연결되는 경우)
	if (isContain(Other, pos))
		return true;

	//그 외의 경우 주어진 범위 내에 있어야하며, 진행하는 방향 양쪽에 복도가 없어야한다(두께를 1로 유지하기 위해)
	if (area.isContain(pos) &&
		!isContain(Other, pos.x + dx, pos.y + dy) &&
		!isContain(Other, pos.x - dx, pos.y - dy))
	{
		return true;
	}

	return false;
}

// 너비 분할 함수
bool BspTreeNode::WidthDivision(float Range, RandomGet& Random)
{
	uniform_real_distribution<float> Ran(0.5f - Range, 0.5f + Range);

	int leftWidth = static_cast<int>(Ran(Random) * m_RoomInfo.m_Width);
	int rightWidth = m_RoomInfo.m_Width - leftWidth;

	if (leftWidth < MINIMUMSIZEX || rightWidth < MINIMUMSIZEX)
		return false;

	m_LeftChild.reset(new BspTreeNode(m_RoomInfo.m_X, m_RoomInfo.m_Y, leftWidth, m_RoomInfo.m_Height));
	m_RightChild.reset(new BspTreeNode(m_RoomInfo.m_X + leftWidth, m_RoomInfo.m_Y, rightWidth, m_RoomInfo.m_Height));
	m_IsWidth = true;

	return true;
}

// 높이 분할 함수
bool BspTreeNode::HeightDivision(float Range, RandomGet& Random)
{
	uniform_real_distribution<float> Ran(0.5f - Range, 0.5f + Range);

	int topHeight = static_cast<int>(Ran(Random) * m_RoomInfo.m_Height);
	int bottomHeight = m_RoomInfo.m_Height - topHeight;

	if (topHeight < MINIMUMSIZEX || bottomHeight < MINIMUMSIZEX)
		return false;

	m_LeftChild.reset(new BspTreeNode(m_RoomInfo.m_X, m_RoomInfo.m_Y, m_RoomInfo.m_Width, topHeight));
	m_RightChild.reset(new BspTreeNode(m_RoomInfo.m_X, m_RoomInfo.m_Y + topHeight, m_RoomInfo.m_Width, bottomHeight));
	m_IsWidth = false;

	return true;
}

// 방들을 연결 시켜주는 함수 이다.
void BspTreeNode::Connect(int Com, const vector<Room*>& leftChild, const vector<Room*>& RightChild, RandomGet& Random)
{
	vector<Point> ways;
	GetAllWay(ways);

	vector<RoomData> rooms;
	GetAllRoom(rooms);

	Point beginDoor;
	Point endDoor;
	Point beginWay, endWay;
	vector<Point> visited;

	RoomData area;
	int beginRoomIdx;
	int endRoomIdx;
	int prevSize = ways.size();

	do
	{
		ways.resize(prevSize);

		uniform_int_distribution<int> beginDist(0, leftChild.size() - 1);
		beginRoomIdx = beginDist(Random);
		auto& begin = *leftChild[beginRoomIdx];

		uniform_int_distribution<int> endDist(0, RightChild.size() - 1);
		endRoomIdx = endDist(Random);
		auto& end = *RightChild[endRoomIdx];

		beginDoor = GetRandomDoor(begin, true, Random);
		endDoor = GetRandomDoor(end, false, Random);
		visited.clear();

		beginWay = getDoorNextPos(beginDoor, begin);
		endWay = getDoorNextPos(endDoor, end);

		int ax = min(beginWay.x, endWay.x) - 2;
		int ay = min(beginWay.y, endWay.y) - 2;
		int awidth = max(beginWay.x, endWay.x) - ax + 3;
		int aheight = max(beginWay.y, endWay.y) - ay + 3;
		area = RoomData(ax, ay, awidth, aheight);

	} while (!isConnect(beginWay, endWay, ways, visited) &&
		!MakeWay(beginWay, endWay, area, Com, rooms, visited, ways, Random));
	//이미 두 방을 연결하는 복도가 존재하거나, 두 방 사이에 복도를 만드는 것에 성공하면 빠져나옴.

	leftChild[beginRoomIdx]->m_Doors.emplace_back(beginDoor);
	RightChild[endRoomIdx]->m_Doors.emplace_back(endDoor);
}

// 빈 공간을 탐색하며 복도 생성(깊이 우선 탐색 알고리즘 적용)
bool BspTreeNode::MakeWay(Point StartPos, Point EndPos, const RoomData & area, int Com, const vector<RoomData>& Rooms, vector<Point>& visite, vector<Point>& OtherWay, RandomGet& Random)
{
	RoomData rc(m_RoomInfo.m_X + 1, m_RoomInfo.m_Y + 1, m_RoomInfo.m_Width - 2, m_RoomInfo.m_Height - 2);
	if (!rc.isContain(StartPos))
	{
		return false;
	}

	visite.emplace_back(StartPos);
	OtherWay.emplace_back(StartPos);

	if (StartPos.x == EndPos.x && StartPos.y == EndPos.y)
	{
		m_Way.emplace_back(StartPos);
		return true;
	}

	//주변 4 방향 테스트.

	vector<Point> pos;
	Point left = Point{ StartPos.x - 1, StartPos.y };
	Point up = Point{ StartPos.x, StartPos.y + 1 };
	Point right = Point{ StartPos.x + 1, StartPos.y };
	Point down = Point{ StartPos.x, StartPos.y - 1 };

	if (isHallPos(left, DIR_LEFT, area, Rooms, OtherWay, visite))
		pos.emplace_back(left);

	if (isHallPos(up, DIR_TOP, area, Rooms, OtherWay, visite))
		pos.emplace_back(up);

	if (isHallPos(right, DIR_RIGHT, area, Rooms, OtherWay, visite))
		pos.emplace_back(right);

	if (isHallPos(down, DIR_BOTTOM, area, Rooms, OtherWay, visite))
		pos.emplace_back(down);

	sort(pos.begin(), pos.end(), [&EndPos](const Point& a, const Point& b)
		{
			int l = (a.x - EndPos.x) * (a.x - EndPos.x) + (a.y - EndPos.y) * (a.y - EndPos.y);
			int r = (b.x - EndPos.x) * (b.x - EndPos.x) + (b.y - EndPos.y) * (b.y - EndPos.y);

			return l < r;
		});

	//정렬된 위치에서, 복잡도에 따라 배열의 일부분만 랜덤하게 섞는다.
	int offset = max<int>(0, pos.size() - Com - 1);

	shuffle(pos.begin() + offset, pos.end(), Random);

	for (auto& c : pos)
	{
		if (MakeWay(c, EndPos, area, Com, Rooms, visite, OtherWay, Random))
		{
			m_Way.emplace_back(StartPos);
			return true;
		}
	}

	return false;
}
// 방에서 랜덤하게 문을 하나 선택해준다.
Point BspTreeNode::GetRandomDoor(const Room & room, bool isBegin, RandomGet& Random)
{
	vector<Point> pos;

	if (m_IsWidth)
	{
		int ys[2] = { room.m_Y, room.getBottom() };

		for (int idx = 0; idx < 2; idx++)
		{
			int y = ys[idx];

			for (int x = room.m_X + 1; x < room.getRight(); x++)
			{
				//문이 연달아 2개가 붙어 있으면 이상함.
				if (isContain(room.m_Doors, Point{ x - 1, y }) ||
					isContain(room.m_Doors, Point{ x + 1, y }))
				{
					continue;
				}

				pos.emplace_back(Point{ x, y });
			}
		}

		int x;

		if (isBegin)
			x = room.getRight();
		else
			x = room.m_X;

		for (int y = room.m_Y + 1; y < room.getBottom(); y++)
		{
			//문이 연달아 2개가 붙어 있으면 이상함.
			if (isContain(room.m_Doors, Point{ x, y - 1 }) ||
				isContain(room.m_Doors, Point{ x, y + 1 }))
			{
				continue;
			}

			pos.emplace_back(Point{ x, y });
		}
	}
	else // x,y 대칭이고 코드는 동일.
	{
		int xs[2] = { room.m_X, room.getRight() };

		for (int idx = 0; idx < 2; idx++)
		{
			int x = xs[idx];

			for (int y = room.m_Y + 1; y < room.getBottom(); y++)
			{
				//문이 연달아 2개가 붙어 있으면 이상함.
				if (isContain(room.m_Doors, Point{ x, y - 1 }) ||
					isContain(room.m_Doors, Point{ x, y + 1 }))
				{
					continue;
				}

				pos.emplace_back(Point{ x, y });
			}
		}

		int y;

		if (isBegin)
			y = room.getBottom();
		else
			y = room.m_Y;

		for (int x = room.m_X + 1; x < room.m_X + room.m_Width - 1; x++)
		{
			//문이 연달아 2개가 붙어 있으면 이상함.
			if (isContain(room.m_Doors, Point{ x - 1, y }) ||
				isContain(room.m_Doors, Point{ x + 1, y }))
			{
				continue;
			}

			pos.emplace_back(Point{ x, y });
		}
	}

	uniform_int_distribution<int> candDist(0, pos.size() - 1);
	Point door = pos[candDist(Random)];

	return door;
}

BspTreeNode * BspTreeNode::GetRightChile()
{
	return m_RightChild.get();
}

BspTreeNode * BspTreeNode::GetLeftChile()
{
	return m_LeftChild.get();
}

bool BspTreeNode::IsHasChile()
{
	return m_LeftChild != nullptr || m_RightChild != nullptr;
}

// 데이터를 채워주는 함수
void BspTreeNode::FillData(int Width, int Height, vector<AREASTATE>& Data, vector<Room *>& Rooms)
{
	if (m_LeftChild != nullptr)
		m_LeftChild->FillData(Width, Height, Data, Rooms);

	if (m_RightChild != nullptr)
		m_RightChild->FillData(Width, Height, Data, Rooms);

	for (auto& p : m_Way)
	{
		Data[p.x + p.y * Width] = AREA_LOAD;
	}

	if (!IsHasChile())
	{
		m_Room.FillData(Width, Height, Data, Rooms);
	}
}
void BspTreeNode::FillObjectData(int Width, int Height, const vector<AREASTATE>& Data, map<int, OBJECTSTATE>& objdata)
{

	if (m_LeftChild != nullptr)
		m_LeftChild->FillObjectData(Width, Height, Data, objdata);

	if (m_RightChild != nullptr)
		m_RightChild->FillObjectData(Width, Height, Data, objdata);

	if (!IsHasChile())
	{
		m_Room.FiilObjectData(Width, Height, Data, objdata);
	}
}
// 방을 합쳐 하나의 맵을 구성하는 함수(복잡도, 랜덤 변수)
void BspTreeNode::Coalescence(int Com, RandomGet& Random)
{
	//자식이 없으면 아무것도 하지 않는다.
	if (!IsHasChile())
		return;

	// 먼저 자식들부터 연결시켜야지
	if (m_LeftChild != nullptr)
		m_LeftChild->Coalescence(Com, Random);

	if (m_RightChild != nullptr)
		m_RightChild->Coalescence(Com, Random);

	//왼쪽과 오른쪽을 연결한다.
	if (m_LeftChild == nullptr || m_RightChild == nullptr)
		return;

	vector<Room*> left;
	vector<Room*> right;

	if (m_IsWidth)
	{
		m_LeftChild->GetSideRoom(DIR_RIGHT, left);
		m_RightChild->GetSideRoom(DIR_LEFT, right);
	}
	else
	{
		m_LeftChild->GetSideRoom(DIR_BOTTOM, left);
		m_RightChild->GetSideRoom(DIR_TOP, right);
	}

	//이미 방이 연결된 상태인지 확인
	bool IsConnected = false;
	for (auto& leftRoom : left)
	{
		for (auto& rightRoom : right)
		{
			if (leftRoom->isConnect(*rightRoom))
			{
				IsConnected = true;
				leftRoom->m_ConnectedRooms.emplace_back(rightRoom);
				rightRoom->m_ConnectedRooms.emplace_back(leftRoom);
			}
		}
	}

	if (IsConnected)
		return;

	Connect(Com, left, right, Random);
}

// 방을 만들어주는 함수(방 사이즈, 사이즈 범위, 랜덤 변수)
// 참고로 방 사이즈와 범위는 합쳐서 1이상이 되어서는 안된다. 전체 크기는 1로 계산
void BspTreeNode::RoomMake(float MidSize, float Range, RandomGet& Random)
{
	if (MidSize - Range < 0.0f || MidSize + Range > 1.0f)
		return;

	if (IsHasChile())
	{
		if (m_LeftChild != nullptr)
			m_LeftChild->RoomMake(MidSize, Range, Random);

		if (m_RightChild != nullptr)
			m_RightChild->RoomMake(MidSize, Range, Random);

		return;
	}
	
	uniform_real_distribution<float> sizeRandom(MidSize - Range, MidSize + Range);
	int roomWidth = static_cast<int>(sizeRandom(Random) * m_RoomInfo.m_Width);
	int roomHeight = static_cast<int>(sizeRandom(Random) * m_RoomInfo.m_Height);

	if (roomWidth < MINIMUMSIZEROOM)
		roomWidth = MINIMUMSIZEROOM;

	if (roomHeight < MINIMUMSIZEROOM)
		roomHeight = MINIMUMSIZEROOM;

	uniform_int_distribution<int> xRan(m_RoomInfo.m_X, m_RoomInfo.getRight() + 1 - roomWidth);
	uniform_int_distribution<int> yRan(m_RoomInfo.m_Y, m_RoomInfo.getBottom() + 1 - roomHeight);

	m_Room.m_X = xRan(Random);
	m_Room.m_Y = yRan(Random);
	m_Room.m_Width = roomWidth;
	m_Room.m_Height = roomHeight;
}

// 맵을 분할 시켜주는 함수(분할 범위)
// 범위가 0.1보다 작거나 0.4보다 크다면 분할 하지 않는다.
void BspTreeNode::Division(float Range, RandomGet& Random)
{
	if (Range < 0.1f || Range > 0.4f)
		return;

	//이미 분할된 노드
	if (IsHasChile())
		return;

	uniform_int_distribution<int> Dist(0, 1);

	//분할 방향 결정.
	if (Dist(Random) == 0) // 너비 절반 분할
	{
		if (!WidthDivision(Range, Random))
		{
			HeightDivision(Range, Random);
		}
	}
	else //높이 절반 분할
	{
		if (!HeightDivision(Range, Random))
		{
			WidthDivision(Range, Random);
		}
	}
}
// 최초로 리셋 시켜주는 함수
void BspTreeNode::Clear(int x, int y, int width, int height)
{
	m_RoomInfo.m_X = x;
	m_RoomInfo.m_Y = y;
	m_RoomInfo.m_Width = width;
	m_RoomInfo.m_Height = height;

	m_Way.clear();
	m_Room = Room();
	m_LeftChild.reset(nullptr);
	m_RightChild.reset(nullptr);
}

BspTreeNode::BspTreeNode(int x, int y, int width, int height)
	:m_RoomInfo(x, y, width, height), m_Room(), m_LeftChild(nullptr), m_RightChild(nullptr)
{
	
}
