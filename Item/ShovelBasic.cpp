#include "stdafx.h"
#include "ShovelBasic.h"


ShovelBasic::ShovelBasic()
{
}


ShovelBasic::~ShovelBasic()
{
}
void ShovelBasic::EffectRender()
{
	if (Getm_IsEffect())
	{
		m_EffectDelayTime += g_ETime;
		Image * img = IMGMANAGER->GetImage("shovelbasic");
		img->Scale(50, 50);
		img->Position(Getm_Pos().x * 50, (Getm_Pos().y * 50) + 30);
		img->Rotate(0, 0, m_RotateY);
		img->Render();
		if (m_EffectDelayTime > 0.2f)
		{
			Setm_IsEffect(false);
			m_MovEffect = false;
			m_EffectDelayTime = 0.0f;
		}
	}
}
void ShovelBasic::Render()
{
	InventoryRender("shovelbasic");
}

void ShovelBasic::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
}

bool ShovelBasic::GetItem(Player * player)
{
	return true;
}

