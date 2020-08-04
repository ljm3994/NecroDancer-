#include "stdafx.h"
#include "Chainmail.h"


Chainmail::Chainmail()
{
}


Chainmail::~Chainmail()
{
}

bool Chainmail::GetItem(Player * player)
{
	if (player->Getm_inven()[7].ItemKey != "")
	{
		Setm_SetInventory(true);
		player->Setm_FramY(2);
		player->Setm_Deffence(player->Getm_Deffence() + m_Deffence);
		ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(player->Getm_Pos().x),
			static_cast<UINT>(player->Getm_Pos().y));
		if (tile->Object2 == OBJECT_ITEM)
		{
			tile->Object2 = OBJECT_EMPTY;
			tile->ImagKey2 = "";
		}
		if (player->Getm_inven()[2].ItemKey != "" && player->Getm_inven()[2].ItemKey != m_Key)
		{
			player->Setm_Deffence(player->Getm_Deffence() - ITEMMANAGER->FindItem(player->Getm_inven()[2].ItemKey)->GetItemDmg());
			ITEMMANAGER->FindItem(player->Getm_inven()[2].ItemKey)->Setm_SetInventory(false);
			ITEMMANAGER->FindItem(player->Getm_inven()[2].ItemKey)->PutItem(player->Getm_Pos(), 1.0f);
		}

		player->Getm_inven()[2].ItemKey = m_Key;
		return true;
	}
	return false;
}

void Chainmail::Render()
{
	InventoryRender("Chainmail");
}

void Chainmail::EffectRender()
{
}

void Chainmail::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	Image * img = IMGMANAGER->GetImage("Chainmail");
	img->Color(0, 0, 0, m_Alpha);
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(50, 50);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = 0;
	img->Render();
}
