#include "stdafx.h"
#include "MoveTrap.h"


MoveTrap::MoveTrap()
{
}


MoveTrap::~MoveTrap()
{
}

void MoveTrap::Update()
{
}

void MoveTrap::Render(float Alpha)
{
	if (TrapUse)
	{
		m_Delay += g_ETime;

		if (m_Delay > 0.2f)
		{
			m_Delay = 0.0f;
			Frame--;
			if (Frame < 0)
			{
				Frame = 1;
				TrapUse = false;
			}
		}
	}
	Image * img = IMGMANAGER->GetImage("MoveTrap");
	img->Scale(40, 40);
	img->Position(m_pos.x * 50, m_pos.y * 50);
	img->GetRefCurrentFrameX() = static_cast<UINT>(Frame);
	img->GetRefCurrentFrameY() = 0;
	img->Rotate(0, 0, Rotate);
	img->Color(0, 0, 0, Alpha);
	img->Render();
}
void MoveTrap::SettingMove(MOVEDIR Dir)
{
	m_Dir = Dir;
	Frame = 1;
	switch (m_Dir)
	{
	case MOVEDIR::MOVE_LEFT:
		Rotate = PI / 2;
		break;
	case MOVEDIR::MOVE_RIGHT:
		Rotate = PI * 2;
		break;
	case MOVEDIR::MOVE_UP:
		Rotate = 0;
		break;
	case MOVEDIR::MOVE_DOWN:
		Rotate = PI;
		break;
	}
}

void MoveTrap::TrapDown(Charter * m_chr)
{
	SOUNDMANAGER->play("Trapbounce", GAMEMANAGER->GetsfxVolum());
	ST_TILE * tile = nullptr;
	switch (m_Dir)
	{
	case  MOVEDIR::MOVE_LEFT:
	{
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(m_pos.x - 1), static_cast<UINT>(m_pos.y));
		m_chr->MoveTrap(m_Dir, tile);
	}
		break;
	case  MOVEDIR::MOVE_RIGHT:
	{
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(m_pos.x + 1), static_cast<UINT>(m_pos.y));
		m_chr->MoveTrap(m_Dir, tile);
	}
		break;
	case  MOVEDIR::MOVE_UP:
	{
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(m_pos.x), static_cast<UINT>(m_pos.y + 1));
		m_chr->MoveTrap(m_Dir, tile);
	}
		break;
	case  MOVEDIR::MOVE_DOWN:
	{
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(m_pos.x), static_cast<UINT>(m_pos.y - 1));
		m_chr->MoveTrap(m_Dir, tile);
	}
		break;
	}
}
