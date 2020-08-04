#pragma once
#include "GameNode.h"
enum MOVEDIR
{
	MOVE_NO,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_UP
};

class Charter : public GameNode
{
protected:
	bool m_BeatTime;
	bool m_PerfectBeat;
	bool m_isJumpMoving;
	bool m_TileCheck = false;
	float m_Jump;
	float RatateY;
	int m_Life;
	int m_MaxLife;
	int m_Deffence;
	bool m_Alive;
	ST_TILE * CurrentTile;
	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_ScreenPos;
	float m_Cnt;
	int m_HeightJump;
	MOVEDIR m_Dir;
public:
	Charter();
	~Charter();
	void MoveTrap(MOVEDIR dir, ST_TILE * Tile);
	virtual void Move(MOVEDIR dir) = 0;
	virtual void Attack() = 0;
	virtual void Damage(int Attack) = 0;
	virtual void Deffence(int& Attack) = 0;
	virtual bool Die() = 0;
	MAKEGETSET(bool, m_isJumpMoving);
	MAKEGETSET(bool, m_Alive);
	MAKEGETSET(int, m_Deffence);
	MAKEGETSET(int, m_Life);
};

