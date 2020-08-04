#include "stdafx.h"
#include "TrapManager.h"
#include "SpikeTrap.h"
#include "MoveTrap.h"
#include "Box.h"

TrapManager::TrapManager()
{
}


TrapManager::~TrapManager()
{
}

string TrapManager::AddTrap(TrapBase * base)
{
	char Index[256];
	sprintf_s(Index, "Trap_%d", Traps.size());
	Traps.insert(make_pair(Index, base));
	return Index;
}

void TrapManager::CreateTrap()
{
	vector<Room *> Rooms = MAPMAKERMANAGER->GetRooms();
	double Percent[3] = { 40.0f, 40.0f, 20.0f };
	for (auto r : Rooms)
	{
		if (r->m_Property != ROOM_PLAYER && r->m_Property != ROOM_SHOP && r->m_Property != ROOM_EXIT)
		{
			random_device rd;
			RandomGet Random(rd());
			uniform_int_distribution<int> Ran(0, 5);
			int TrapNum = Ran(Random);
			for (int i = 0; i < TrapNum; i++)
			{
				uniform_int_distribution<int> Xpos(r->m_X, r->getRight());
				uniform_int_distribution<int> Ypos(r->m_Y, r->getBottom());
				int RanXpos = Xpos(Random);
				int RanYpos = Ypos(Random);
				if (MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos)->Object == OBJECT_EMPTY &&
					MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos)->Object2 == OBJECT_EMPTY &&
					MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos)->Object3 == OBJECT_EMPTY)
				{
					
					int Select = GetRandom(Percent, 3);
					TrapBase * tBase = nullptr;
					TileProperty Property;
					TileObject ob;
					switch (Select)
					{
					case 1:
					{
						tBase = OBJECTPOOL(SpikeTrap)->GetObObject();
						Property = PASS;
						ob = OBJECT_TRAP;
					}
						break;
					case 2:
					{
						uniform_int_distribution<int> MoveRan(1, 4);
						int MoveTrapRan = MoveRan(Random);
						tBase = OBJECTPOOL(MoveTrap)->GetObObject();
						((MoveTrap*)tBase)->SettingMove((MOVEDIR)MoveTrapRan);
						Property = PASS;
						ob = OBJECT_TRAP;
					}
						break;
					case 3:
					{
						tBase = OBJECTPOOL(Box)->GetObObject();
						Property = NONPASS;
						ob = OBJECT_ITEMBOX;
					}
					break;
					}
					tBase->TrapSetting(MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos), D3DXVECTOR2(
						static_cast<float>(RanXpos), static_cast<float>(RanYpos)));
					string Index = AddTrap(tBase);
					MAPMANAGER->GetAllMap()->ObjectsetMap(RanXpos, RanYpos, 0, 0, MODE_DRAW, Index, false, Property, ob);
				}
			}
		}
	}
}

TrapBase * TrapManager::FindTrap(string Index)
{
	return Traps.find(Index)->second;

}
