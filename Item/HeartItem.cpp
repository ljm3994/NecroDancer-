#include "stdafx.h"
#include "HeartItem.h"


HeartItem::HeartItem()
{
}


HeartItem::~HeartItem()
{
}

HRESULT HeartItem::Init()
{
	Setm_IsEffect(false);
	return S_OK;
}

void HeartItem::Update()
{
}

void HeartItem::EffectRender()
{
}

void HeartItem::Render()
{
}

int HeartItem::GetItemDmg()
{
	return 0;
}

void HeartItem::PutItem(D3DXVECTOR2 pos, float Alpha)
{

}

void HeartItem::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	Image * img = IMGMANAGER->GetImage("HeartItem");
	img->Color(0, 0, 0, m_Alpha);
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(50, 50);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = 0;
	img->Render();
}

bool HeartItem::GetItem(Player * player)
{
	if (player->Getm_MaxLife() <= 20)
	{
		player->Setm_MaxLife(player->Getm_MaxLife() + 2);
		player->Setm_Life(player->Getm_MaxLife());
		return true;
	}
	return false;
}
