#include "stdafx.h"
#include "Charter.h"


Charter::Charter()
{
}


Charter::~Charter()
{
}

void Charter::MoveTrap(MOVEDIR dir, ST_TILE * Tile)
{
	if (Tile->Property == TileProperty::PASS)
	{
		switch (dir)
		{
		case MOVE_LEFT:
			m_Pos.x -= 1;
			m_Jump = 0;
			break;
		case MOVE_RIGHT:
			m_Pos.x += 1;
			m_Jump = 0;
			break;
		case MOVE_UP:
			m_Pos.y += 1;
			m_Jump = 1;
			break;
		case MOVE_DOWN:
			m_Pos.y -= 1;
			m_Jump = 0;
			break;
		}
		m_Cnt = 50;
		CurrentTile = Tile;
		m_TileCheck = false;
		
	}
}


