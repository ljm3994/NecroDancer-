#include "stdafx.h"
#include "SpikeTrap.h"


SpikeTrap::SpikeTrap()
{
}


SpikeTrap::~SpikeTrap()
{
}


void SpikeTrap::Update()
{
}

void SpikeTrap::Render(float Alpha)
{
	if (TrapUse)
	{
		m_Delay += g_ETime;

		if (m_Delay > 0.2f)
		{
			m_Delay = 0.0f;
			Frame++;
			if (Frame > 2)
			{
				Frame = 0;
				TrapUse = false;
			}
		}
	}
	Image * img = IMGMANAGER->GetImage("SpikeTrap");
	img->Scale(50, 50);
	img->Position(m_pos.x * 50, m_pos.y * 50);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = static_cast<UINT>(Frame);
	img->Render();
}

void SpikeTrap::TrapDown(Charter * m_chr)
{
	SOUNDMANAGER->play("Trapspike", GAMEMANAGER->GetsfxVolum());
	Frame = 0;
	m_chr->Setm_Life(m_chr->Getm_Life() - 2);
	TrapUseCharter(true);
}
