#include "stdafx.h"
#include "BspTreeNode.h"

// ������ ���� �ֺ��� ��� ���� ����ִ� �Լ�
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

// ��� ���� �������� �Լ�
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

// ��� ��θ� �������� �Լ�
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

// ���� �� ��ġ���� ���� ���� ��ġ�� ��ȯ���ش�
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

// ���� ����Ǿ���� Ȯ�� �ϴ� �Լ�
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

// ���� ����Ǿ��ִ� �����Ϳ� ������ ���� ����ִ��� Ȯ�� �ϴ� �Լ�
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

// ������ ��ġ�� �������� �Լ�(�ش� �Լ� �������� ������ �β��� ������ �� �ִ�)
bool BspTreeNode::isHallPos(const Point & pos, DIRECTION dir, RoomData area, const vector<RoomData>& rooms, const vector<Point>& Other, const vector<Point>& visite)
{
	int dx, dy;

	// �� �ʿ� ��ΰ� �̹� �����ϴ� Ȯ�� �� �� �ִ�.
	// 1�� �����ϴ� ������ ���� ����� �β��� 1�̱� �����̴�.
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

	//��� ��ġ�ų� �湮�ߴ� �� �� �湮�ϸ� �� �ǹǷ� �� �ΰ�츦 ���� üũ�ؼ� �����Ѵ�.
	if (isContain(rooms, pos) || isContain(visite, pos))
		return false;

	//�̹� �����ϴ� ���� ���� �ִ� ��� �����Ѵ�(�ش� ������ ����Ǵ� ���)
	if (isContain(Other, pos))
		return true;

	//�� ���� ��� �־��� ���� ���� �־���ϸ�, �����ϴ� ���� ���ʿ� ������ ������Ѵ�(�β��� 1�� �����ϱ� ����)
	if (area.isContain(pos) &&
		!isContain(Other, pos.x + dx, pos.y + dy) &&
		!isContain(Other, pos.x - dx, pos.y - dy))
	{
		return true;
	}

	return false;
}

// �ʺ� ���� �Լ�
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

// ���� ���� �Լ�
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

// ����� ���� �����ִ� �Լ� �̴�.
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
	//�̹� �� ���� �����ϴ� ������ �����ϰų�, �� �� ���̿� ������ ����� �Ϳ� �����ϸ� ��������.

	leftChild[beginRoomIdx]->m_Doors.emplace_back(beginDoor);
	RightChild[endRoomIdx]->m_Doors.emplace_back(endDoor);
}

// �� ������ Ž���ϸ� ���� ����(���� �켱 Ž�� �˰��� ����)
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

	//�ֺ� 4 ���� �׽�Ʈ.

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

	//���ĵ� ��ġ����, ���⵵�� ���� �迭�� �Ϻκи� �����ϰ� ���´�.
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
// �濡�� �����ϰ� ���� �ϳ� �������ش�.
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
				//���� ���޾� 2���� �پ� ������ �̻���.
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
			//���� ���޾� 2���� �پ� ������ �̻���.
			if (isContain(room.m_Doors, Point{ x, y - 1 }) ||
				isContain(room.m_Doors, Point{ x, y + 1 }))
			{
				continue;
			}

			pos.emplace_back(Point{ x, y });
		}
	}
	else // x,y ��Ī�̰� �ڵ�� ����.
	{
		int xs[2] = { room.m_X, room.getRight() };

		for (int idx = 0; idx < 2; idx++)
		{
			int x = xs[idx];

			for (int y = room.m_Y + 1; y < room.getBottom(); y++)
			{
				//���� ���޾� 2���� �پ� ������ �̻���.
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
			//���� ���޾� 2���� �پ� ������ �̻���.
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

// �����͸� ä���ִ� �Լ�
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
// ���� ���� �ϳ��� ���� �����ϴ� �Լ�(���⵵, ���� ����)
void BspTreeNode::Coalescence(int Com, RandomGet& Random)
{
	//�ڽ��� ������ �ƹ��͵� ���� �ʴ´�.
	if (!IsHasChile())
		return;

	// ���� �ڽĵ���� ������Ѿ���
	if (m_LeftChild != nullptr)
		m_LeftChild->Coalescence(Com, Random);

	if (m_RightChild != nullptr)
		m_RightChild->Coalescence(Com, Random);

	//���ʰ� �������� �����Ѵ�.
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

	//�̹� ���� ����� �������� Ȯ��
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

// ���� ������ִ� �Լ�(�� ������, ������ ����, ���� ����)
// ����� �� ������� ������ ���ļ� 1�̻��� �Ǿ�� �ȵȴ�. ��ü ũ��� 1�� ���
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

// ���� ���� �����ִ� �Լ�(���� ����)
// ������ 0.1���� �۰ų� 0.4���� ũ�ٸ� ���� ���� �ʴ´�.
void BspTreeNode::Division(float Range, RandomGet& Random)
{
	if (Range < 0.1f || Range > 0.4f)
		return;

	//�̹� ���ҵ� ���
	if (IsHasChile())
		return;

	uniform_int_distribution<int> Dist(0, 1);

	//���� ���� ����.
	if (Dist(Random) == 0) // �ʺ� ���� ����
	{
		if (!WidthDivision(Range, Random))
		{
			HeightDivision(Range, Random);
		}
	}
	else //���� ���� ����
	{
		if (!HeightDivision(Range, Random))
		{
			WidthDivision(Range, Random);
		}
	}
}
// ���ʷ� ���� �����ִ� �Լ�
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
