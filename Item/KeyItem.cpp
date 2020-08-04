#include "stdafx.h"
#include "KeyItem.h"


KeyItem::KeyItem()
{
}


KeyItem::~KeyItem()
{
}

HRESULT KeyItem::Init()
{
	Setm_IsEffect(false);
	return S_OK;
}

void KeyItem::Update()
{
}

void KeyItem::Render()
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage("LockKey");
		img->Position(180 + g_ptCam.x, (WINSIZEY - 190) + g_ptCam.y);
		img->Color(0, 0, 0, 1.0f);
		img->Scale(60, 60);
		img->Render();
	}
}

void KeyItem::EffectRender()
{
}

int KeyItem::GetItemDmg()
{
	return 0;
}

void KeyItem::PutItem(D3DXVECTOR2 pos, float Alpha)
{

}

void KeyItem::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	Image * img = IMGMANAGER->GetImage("LockKey");
	img->Color(0, 0, 0, m_Alpha);
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(50, 50);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = 0;
	img->Render();
}

bool KeyItem::GetItem(Player * player)
{
	Setm_SetInventory(true);
	player->Getm_inven()[6].ItemKey = m_Key;
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(player->Getm_Pos().x),
		static_cast<UINT>(player->Getm_Pos().y));
	tile->Object2 = OBJECT_EMPTY;
	tile->ImagKey2 = "";
	return true;
}
