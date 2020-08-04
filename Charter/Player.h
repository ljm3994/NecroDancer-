#pragma once
#include "Charter.h"
enum InvenKind;
struct Inventory
{
	InvenKind Kind;
	string ItemKey;

	Inventory(): Kind((InvenKind)0), ItemKey(""){}
};

class Player : public Charter
{
	Image * m_Head;
	Image * m_Body;
	Image * m_Shadow;
	int m_EarthquakeCnt;
	Inventory * m_inven;
	bool m_Mujeck;
	bool RPLUSL;
	MOVEDIR Dir;
	bool m_IsShove;
	bool m_IsEarthquake;
	bool m_PlayerKeyOn;
	float m_DelayTime;
	float m_EarthquakeDelay;
	int m_FramX;
	int m_FramY;
	int m_Coin;
	int m_Diamond;
	int m_PlayerSight;
	float m_BodyAniDelayTime;
public:
	Player();
	~Player();
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void Move(MOVEDIR dir) override;
	void Attack() override;
	void Damage(int Attack) override;
	void Deffence(int& Attack) override;
	void SetInvenTory(int Num, string str);
	void TileCheck();
	void PlayerInitPos();
	void PlayerRayCast();
	void Earthquake();
	void MoveCheck(ST_TILE * Tile, MOVEDIR dir);
	bool Die() override;
	MAKEGETSET(D3DXVECTOR2, m_Pos);
	MAKEGETSET(D3DXVECTOR2, m_ScreenPos);
	MAKEGETSET(bool, m_IsEarthquake);
	MAKEGETSET(bool, m_BeatTime);
	MAKEGETSET(bool, m_PerfectBeat);
	MAKEGETSET(bool, m_PlayerKeyOn);
	MAKEGETSET(int, m_PlayerSight);
	MAKEGETSET(ST_TILE*, CurrentTile);
	MAKEGET(Inventory *, m_inven);
	MAKEGETSET(int, m_Life);
	MAKEGETSET(int, m_MaxLife);
	MAKEGETSET(int, m_Coin);
	MAKEGETSET(int, m_Diamond);
	MAKEGETSET(bool, m_IsShove);
	MAKEGET(MOVEDIR, Dir);
	MAKEGETSET(bool, RPLUSL);
	MAKEGETSET(int, m_FramY);
};

