#pragma once
#include "Skeleton.h"
#include "Jombi.h"
#include "Kingkongga.h"
#include "Player.h"
#include "Singleton/SingletonBase.h"
class CharcterManager : public SingletonBase<CharcterManager>
{
	map<string, EnemyBase*> m_Enemy;
	map<string, EnemyBase*>::iterator m_Enemyit;
	Player * m_Player;
	int DieMonster;
	bool m_IsBeat;
	bool m_CreateCharter;
	int m_MaxEnemyNum;
	int m_CurrentEnemyNum;
public:
	CharcterManager();
	~CharcterManager();
	void Init(int MaxEnemyNum);
	void Render();
	void PlayerInit();
	string AddEnemy(EnemyBase * Enemy);
	void AllMonsterTileSetting();
	void PlayerRender();
	D3DXVECTOR2 GetPos() { return m_Player->Getm_Pos(); }
	void CreateEnemy(int Attack, int Life, int Deffence);
	void CreateJombe(int MaxNum, D3DXVECTOR2 m_Pos);
	void CreateCharter();
	void EnemyRender(string index, float Alpha);
	void JombeUpdate();
	void PlayerUpdate();
	void BossCreate();
	void EnemyReset();
	void DeletePlayer();
	int EnemySize() { return m_Enemy.size(); }
	EnemyBase* FindEnemy(string Index);
	Player * GetPlayer() { return m_Player; }
	void Update();
	MAKEGET(bool, m_CreateCharter);
	MAKEGETSET(bool, m_IsBeat);
	MAKEGET(int, DieMonster);
	MAKEGETSET(Player *, m_Player);
};

