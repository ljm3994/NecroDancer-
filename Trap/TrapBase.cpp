#include "stdafx.h"
#include "TrapBase.h"

TrapBase::TrapBase()
{
}


TrapBase::~TrapBase()
{
}

void TrapBase::TrapSetting(ST_TILE * TrapTile, D3DXVECTOR2 Pos)
{
	Frame = 0;
	m_Delay = 0.0f;
	CurrentTile = TrapTile;
	m_pos = Pos;
}

void TrapBase::TrapRemove()
{
	CurrentTile->Object = OBJECT_EMPTY;
	CurrentTile->ImageKey = "";
	CurrentTile->Property = PASS;
}

void TrapBase::TrapUseCharter(bool Use)
{
	TrapUse = Use;
}
