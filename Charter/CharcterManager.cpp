#include "stdafx.h"
#include "Slime.h"
#include "Minotaur.h"
#include "CharcterManager.h"


CharcterManager::CharcterManager()
{
	m_CreateCharter = false;
	m_IsBeat = false;
}


CharcterManager::~CharcterManager()
{
}

void CharcterManager::Init(int MaxEnemyNum)
{
	m_MaxEnemyNum = 50;
	DieMonster = 0;
}

void CharcterManager::Render()
{
	m_Enemyit = m_Enemy.begin();
	for (; m_Enemyit != m_Enemy.end(); m_Enemyit++)
	{
		if (m_Enemyit->second->Getm_Alive())
		{
			m_Enemyit->second->HpRender();
			m_Enemyit->second->AttackRender();
		}
	}
}

void CharcterManager::PlayerInit()
{
	m_Player = new Player();
	m_Player->init();

	m_CreateCharter = true;
}

string CharcterManager::AddEnemy(EnemyBase * Enemy)
{
	char Index[256];
	sprintf_s(Index, "Monster_%d", m_Enemy.size());
	Enemy->SetKey(Index);
	m_Enemy.insert(make_pair(Index, Enemy));
	return Index;
}

void CharcterManager::AllMonsterTileSetting()
{
	m_Enemyit = m_Enemy.begin();
	for (; m_Enemyit != m_Enemy.end(); m_Enemyit++)
	{
		MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Enemyit->second->Getm_Pos().x), static_cast<int>(m_Enemyit->second->Getm_Pos().y), TileObject::OBJECT_ENEMY,TileProperty::NONPASS, m_Enemyit->second->GetKey());
	}
}


void CharcterManager::PlayerRender()
{
	m_Player->render();
}

void CharcterManager::CreateEnemy(int Attack, int Life, int Deffence)
{
	vector<Room *> Rooms = MAPMAKERMANAGER->GetRooms();

	for (auto r : Rooms)
	{
		if (r->m_Property != ROOM_PLAYER && r->m_Property != ROOM_SHOP)
		{
			random_device rd;
			RandomGet Random(rd());
			uniform_int_distribution<int> Ran(0, 6);
			int MonsterNum = Ran(Random);
			for (int i = 0; i < MonsterNum;)
			{
				uniform_int_distribution<int> Xpos(r->m_X, r->getRight());
				uniform_int_distribution<int> Ypos(r->m_Y, r->getBottom());
				int RanXpos = Xpos(Random);
				int RanYpos = Ypos(Random);
				if (MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos)->Object == OBJECT_EMPTY &&
					MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos)->Object2 == OBJECT_EMPTY &&
					MAPMANAGER->GetAllMap()->GetTile(RanXpos, RanYpos)->Object3 == OBJECT_EMPTY)
				{
					uniform_int_distribution<int> EnRan(0, 2);
					EnemyBase * Enemy = nullptr;
					switch (EnRan(Random))
					{
					case 0:
						Enemy = OBJECTPOOL(Skeleton)->GetObObject();
						Enemy->Setm_MyRoom(r);
						Enemy->init();
						Enemy->SettingEnemy(1 + Life, 1 + Life, 1 + Attack, 0 + Deffence);
						Enemy->SetPos(D3DXVECTOR2(static_cast<float>(RanXpos), static_cast<float>(RanYpos)));
						break;
					case 1:
						Enemy = OBJECTPOOL(Slime)->GetObObject();
						Enemy->Setm_MyRoom(r);
						Enemy->init();
						Enemy->SettingEnemy(1 + Life, 1 + Life, 1 + Attack, 0 + Deffence);
						Enemy->SetPos(D3DXVECTOR2(static_cast<float>(RanXpos), static_cast<float>(RanYpos)));
						break;
					case 2:
						Enemy = OBJECTPOOL(Minotaur)->GetObObject();
						Enemy->Setm_MyRoom(r);
						Enemy->init();
						Enemy->SettingEnemy(2 + Life, 2 + Life, 2 + Attack, 0 + Deffence);
						Enemy->SetPos(D3DXVECTOR2(static_cast<float>(RanXpos), static_cast<float>(RanYpos)));
						break;
					}
					string Index = AddEnemy(Enemy);
					MAPMANAGER->GetAllMap()->SetMonsterProperty(RanXpos, RanYpos, TileObject::OBJECT_ENEMY, TileProperty::NONPASS, Index);
					i++;
				}
				else
				{
					continue;
				}
			}
		}
	}
}

void CharcterManager::CreateJombe(int MaxNum, D3DXVECTOR2 m_Pos)
{
	for (int i = 0; i < MaxNum; i++)
	{
		Jombi * CJombe = OBJECTPOOL(Jombi)->GetObObject();
		CJombe->init();
		CJombe->JombeInit();
		CJombe->SettingEnemy(1, 1, 1, 0);
		CJombe->SetPos(D3DXVECTOR2(m_Pos.x, m_Pos.y + i));
		string Index = AddEnemy(CJombe);
		MAPMANAGER->GetAllMap()->SetMonsterProperty(static_cast<int>(m_Pos.x), static_cast<int>(m_Pos.y + i), TileObject::OBJECT_ENEMY, TileProperty::NONPASS, Index);
	}
	int MaxSize = m_Enemy.size() - MaxNum;
	for (int i = 0; i < MaxNum; i++)
	{
		char strIndex[256];
		sprintf_s(strIndex, "Monster_%d", MaxSize + i);
		Jombi * CJombe = static_cast<Jombi*>(FindEnemy(strIndex));
		if (i == 0)
		{
			strIndex[256];
			sprintf_s(strIndex, "Monster_%d", MaxSize + (i + 1));
			CJombe->Setm_Next(static_cast<Jombi*>(FindEnemy(strIndex)));

		}
		else if (i == MaxNum - 1)
		{
			strIndex[256];
			sprintf_s(strIndex, "Monster_%d", MaxSize + (i - 1));
			CJombe->Setm_Before(static_cast<Jombi*>(FindEnemy(strIndex)));
		}
		else
		{
			strIndex[256];
			sprintf_s(strIndex, "Monster_%d", MaxSize + (i + 1));
			CJombe->Setm_Next(static_cast<Jombi*>(FindEnemy(strIndex)));
			strIndex[256];
			sprintf_s(strIndex, "Monster_%d", MaxSize + (i - 1));
			CJombe->Setm_Before(static_cast<Jombi*>(FindEnemy(strIndex)));
		}
	}
}

void CharcterManager::CreateCharter()
{


}

void CharcterManager::EnemyRender(string index, float Alpha)
{
	m_Enemy[index]->Setm_Alpha(Alpha);
	m_Enemy[index]->render();
}

void CharcterManager::JombeUpdate()
{
	if (m_Player->Getm_Alive())
	{
		m_Enemyit = m_Enemy.begin();
		for (; m_Enemyit != m_Enemy.end();)
		{
			if (m_Enemyit->second->Getm_Alive())
			{
				if (m_Enemyit->second->Die())
				{
					m_Enemyit->second->update();
					m_Enemyit->second->AstarPath(m_IsBeat);
				}
				else
				{
					DieMonster++;
				}
			}
			m_Enemyit++;
		}
		m_IsBeat = false;
	}
}

void CharcterManager::PlayerUpdate()
{
	if (m_Player->Getm_Alive())
	{
		m_Player->update();
	}
}

void CharcterManager::BossCreate()
{
	Kingkongga * boss = OBJECTPOOL(Kingkongga)->GetObObject();
	boss->init();
	boss->KingkonggaInit();
	boss->SettingEnemy(4, 4, 2, 0);
	boss->SetPos(D3DXVECTOR2(11, 20));
	string Index = AddEnemy(boss);
	MAPMANAGER->GetAllMap()->SetMonsterProperty(11, 20, TileObject::OBJECT_ENEMY, TileProperty::NONPASS, Index);
}


void CharcterManager::EnemyReset()
{
	m_Enemyit = m_Enemy.begin();
	for (; m_Enemyit != m_Enemy.end();)
	{
		if (m_Enemyit->second)
		{
			m_Enemyit->second->release();
			SAFE_DELETE(m_Enemyit->second);
			//OBJECTPOOL(Skeleton)->PushObject(m_Enemyit->second);
		}
		m_Enemyit++;
	}
	DieMonster = 0;
	m_Enemy.clear();
}

void CharcterManager::DeletePlayer()
{
	m_Player->release();
	SAFE_DELETE(m_Player);
}

EnemyBase * CharcterManager::FindEnemy(string Index)
{
	return m_Enemy[Index];
}

void CharcterManager::Update()
{
	if (m_Player->Getm_Alive())
	{
		m_Player->update();
		m_Enemyit = m_Enemy.begin();
		for (; m_Enemyit != m_Enemy.end();)
		{
			if (m_Enemyit->second->Getm_Alive())
			{
				if (m_Enemyit->second->Die())
				{
					m_Enemyit->second->AstarPath(m_IsBeat);
					m_Enemyit->second->update();
				}
				else
				{
					DieMonster++;
				}
			}
			m_Enemyit++;
		}
		m_IsBeat = false;
	}
}


