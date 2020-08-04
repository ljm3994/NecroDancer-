#pragma once
struct ST_TILE;
class Charter;

class TrapBase
{
protected:
	ST_TILE * CurrentTile;
	D3DXVECTOR2 m_pos;
	float m_Delay;
	float Frame;
	bool TrapUse;
	
public:
	TrapBase();
	~TrapBase();
	void TrapSetting(ST_TILE * TrapTile, D3DXVECTOR2 m_Pos);
	void TrapRemove();
	void TrapUseCharter(bool Use);
	virtual void TrapDown(Charter * m_chr) = 0;
	virtual void Update() = 0;
	virtual void Render(float Alpha) = 0;
	MAKEGETSET(D3DXVECTOR2, m_pos);
};

