#include "stdafx.h"
#include "HeadHelmet.h"


HeadHelmet::HeadHelmet()
{
}


HeadHelmet::~HeadHelmet()
{
}

bool HeadHelmet::GetItem(Player * player)
{
	if (player->Getm_inven()[7].ItemKey != "")
	{
		Setm_SetInventory(true);
		player->Setm_Deffence(player->Getm_Deffence() + m_Deffence);
		ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(player->Getm_Pos().x),
			static_cast<UINT>(player->Getm_Pos().y));
		if (tile->Object2 == OBJECT_ITEM)
		{
			tile->Object2 = OBJECT_EMPTY;
			tile->ImagKey2 = "";
		}
		if (player->Getm_inven()[3].ItemKey != "" && player->Getm_inven()[3].ItemKey != m_Key)
		{
			player->Setm_Deffence(player->Getm_Deffence() - ITEMMANAGER->FindItem(player->Getm_inven()[3].ItemKey)->GetItemDmg());
			ITEMMANAGER->FindItem(player->Getm_inven()[3].ItemKey)->Setm_SetInventory(false);
			ITEMMANAGER->FindItem(player->Getm_inven()[3].ItemKey)->PutItem(player->Getm_Pos(), 1.0f);
		}

		player->Getm_inven()[3].ItemKey = m_Key;
		return true;
	}
	return false;
}

void HeadHelmet::Render()
{
	InventoryRender("Headhelmet");
}

void HeadHelmet::EffectRender()
{
}

void HeadHelmet::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	Image * img = IMGMANAGER->GetImage("Headhelmet");
	img->Color(0, 0, 0, m_Alpha);
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(50, 50);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = 0;
	img->Render();
}
