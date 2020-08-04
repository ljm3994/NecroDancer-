#include "stdafx.h"
#include "BspTreeHelper.h"

vector<Room*> Room::GetAllRooms()
{
	vector<Room*> rooms;
	queue<Room*> queue;

	queue.push(this);

	while (!queue.empty())
	{
		auto now = queue.front();
		queue.pop();

		now->m_IsVisited = true;

		rooms.push_back(now);

		for (auto adj : now->m_ConnectedRooms)
		{
			if (adj->m_IsVisited)
				continue;

			queue.push(adj);
		}
	}

	return rooms;
}

void Room::FillData(int width, int height, vector<AREASTATE>& data, vector<Room *>& Rooms)
{
	if (m_IsVisited)
		return;

	vector<Room*> allRooms = GetAllRooms();

	for (auto r : allRooms)
	{
		auto& room = *r;
		room.m_Property = ROOM_ETC;

		for (int y = room.m_Y; y < room.getBottom() + 1; y++)
		{
			for (int x = room.m_X; x < room.getRight() + 1; x++)
			{
				if (isWallPostion(x, y, width, height, allRooms))
				{
					int Random = GetInt(10);
					if (Random == 0)
					{
						data[x + y * width] = AREA_TORCH;
					}
					else
					{
						data[x + y * width] = AREA_WALL;
					}
				}
				else
				{
					data[x + y * width] = AREA_ROOM;
				}
			}
		}

		for (auto& door : room.m_Doors)
		{
			if (((data[(door.x - 1) + door.y * width] == AREA_WALL || data[(door.x - 1) + door.y * width] == AREA_TORCH)
				&& (data[(door.x + 1) + door.y * width] == AREA_WALL || data[(door.x + 1) + door.y * width] == AREA_TORCH)) 
				|| ((data[door.x + (door.y + 1) * width] == AREA_WALL || data[(door.x) + (door.y + 1) * width] == AREA_TORCH) 
					&& (data[door.x + (door.y - 1) * width] == AREA_WALL || data[door.x + (door.y - 1) * width] == AREA_TORCH)))
			{
				if ((data[(door.x - 1) + door.y * width] == AREA_ROOM || data[(door.x + 1) + door.y * width] == AREA_ROOM) || (data[(door.x - 1) + door.y * width] == AREA_SHOP || data[(door.x + 1) + door.y * width] == AREA_SHOP))
				{
					data[door.x + door.y * width] = AREA_SIDEDOOR;
				}
				else
				{
					data[door.x + door.y * width] = AREA_DOOR;
				}
			}
		}

		Rooms.emplace_back(r);
	}
}

void Room::FiilObjectData(int width, int height, const vector<AREASTATE>& data, map<int, OBJECTSTATE> &obdata)
{
	/*vector<Room*> allRooms = GetAllRooms();
	mt19937 RandomEnging;
	uniform_int_distribution<int> ran(0, 200);

	for (auto r : allRooms)
	{
		const auto& room = *r;
		bool Box = false;
		int TrapCnt = 0;
		for (int y = room.m_Y; y < room.getBottom() + 1; y++)
		{
			for (int x = room.m_X; x < room.getRight() + 1; x++)
			{
				if (x > 0 && x < width - 1 && y > 0 && y < height - 1)
				{
					if ((data[(x - 1) + y * width] == AREA_DOOR) || (data[(x + 1) + y * width] == AREA_DOOR)
						|| (data[x + (y - 1) * width] == AREA_DOOR) || (data[x + (y + 1) * width] == AREA_DOOR))
					{
						continue;
					}
					if ((data[x + y * width]) == AREA_ROOM)
					{
						int r = ran(RandomEnging);

						if (r == 0 && !Box)
						{
							obdata.insert(make_pair<int, OBJECTSTATE>(x + y * width, OBJECTSTATE::OBJECT_BOX));
							Box = true;
						}
						else if (r >= 60 && r <= 62 && TrapCnt < 3)
						{
							obdata.insert(make_pair<int, OBJECTSTATE>(x + y * width, OBJECTSTATE::OBJECT_DOWNTRAP));
							TrapCnt++;
						}
						else if (r >= 100 && r <= 102 && TrapCnt < 3)
						{
							obdata.insert(make_pair<int, OBJECTSTATE>(x + y * width, OBJECTSTATE::OBJECT_MOVETRAP));
							TrapCnt++;
						}
					}
				}
			}
		}

	}*/
}

bool Room::isWallPostion(int x, int y, int width, int height, const vector<Room*>& rooms)
{
	Point EightPos[8] =
	{
		{ x + 1, y },   //오른쪽
		{ x + 1, y + 1 },      //오른쪽 위
		{ x, y + 1 },       // 위
		{ x - 1, y + 1 },          //왼쪽 위
		{ x - 1, y },    //왼쪽
		{ x - 1, y - 1 },     //왼쪽 아래
		{ x, y - 1 },        // 아래
		{ x + 1,y - 1 }      // 오른쪽 아래
	};

	for (int i = 0; i < 8; i++)
	{
		const Point& Temp = EightPos[i];
		if (all_of(rooms.begin(), rooms.end(), [&Temp](const Room* room)
			{
				return !room->isContain(Temp);
			}))
		{
			return true;
		}
	}

	return false;
}

bool RoomData::isConnect(const RoomData & other) const
{
	if (m_X == other.getRight() + 1 ||
		other.m_X == getRight() + 1)
	{
		return (m_Y >= other.m_Y && m_Y < other.getBottom() - 1) ||
			(other.m_Y >= m_Y && other.m_Y < getBottom() - 1);
	}

	if (m_Y == other.m_Y + other.m_Height ||
		other.m_Y == m_Y + m_Height)
	{
		return (m_X >= other.m_X && m_X < other.getRight() - 1) ||
			(other.m_X >= m_X && other.m_X < getRight() - 1);
	}

	return false;
}

bool RoomData::isContain(const Point & pos) const
{
	return pos.x >= m_X && pos.x <= getRight() &&
		pos.y >= m_Y && pos.y <= getBottom();
}
