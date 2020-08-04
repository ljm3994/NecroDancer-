#include "stdafx.h"
#include "BorodSword.h"


BorodSword::BorodSword()
{
}


BorodSword::~BorodSword()
{
}

void BorodSword::Render()
{
	InventoryRender("BroadSword");
}

void BorodSword::EffectRender()
{
	if (Getm_IsEffect())
	{
		Image * img = IMGMANAGER->GetImage("BroadSwordEffect");
		img->Scale(60, 60);
		img->Position(Getm_Pos().x * 50, (Getm_Pos().y * 50) + 20);
		img->Rotate(0, 0, m_RotateY);
		img->PlayAnimation(PlayState::ANI_LOOP, true, 0.2f);
		if (img->GetRefCurrentFrameX() == 2)
		{
			img->GetRefCurrentFrameX() = 0;
			Setm_IsEffect(false);
		}
	}
}

void BorodSword::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	Image * img = IMGMANAGER->GetImage("BroadSword");
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(40, 40);
	img->Color(0, 0, 0, m_Alpha);
	img->Render();
}

bool BorodSword::GetItem(Player * player)
{
	Setm_SetInventory(true);
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(player->Getm_Pos().x), 
		static_cast<UINT>(player->Getm_Pos().y));
	if (tile->Object2 == OBJECT_ITEM)
	{
		tile->Object2 = OBJECT_EMPTY;
		tile->ImagKey2 = "";
	}
	if (player->Getm_inven()[1].ItemKey != "" && player->Getm_inven()[1].ItemKey != m_Key)
	{
		ITEMMANAGER->FindItem(player->Getm_inven()[1].ItemKey)->Setm_SetInventory(false);
		ITEMMANAGER->FindItem(player->Getm_inven()[1].ItemKey)->PutItem(player->Getm_Pos(), 1.0f);
	}

	player->Getm_inven()[1].ItemKey = m_Key;
	return true;
}

void BorodSword::WepponAttack(MOVEDIR dir)
{
	Player * pl = CHARTERMANAGER->GetPlayer();
	ST_TILE * tile = nullptr;
	switch (dir)
	{
	case MOVE_LEFT:
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x - 1), static_cast<UINT>(pl->Getm_Pos().y));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x - 1), static_cast<UINT>(pl->Getm_Pos().y - 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x - 1), static_cast<UINT>(pl->Getm_Pos().y + 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		break;
	case MOVE_RIGHT:
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x + 1), static_cast<UINT>(pl->Getm_Pos().y));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x + 1), static_cast<UINT>(pl->Getm_Pos().y - 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x + 1), static_cast<UINT>(pl->Getm_Pos().y + 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		break;
	case MOVE_UP:
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x), static_cast<UINT>(pl->Getm_Pos().y + 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x - 1), static_cast<UINT>(pl->Getm_Pos().y + 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x + 1), static_cast<UINT>(pl->Getm_Pos().y + 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		break;
	case MOVE_DOWN:
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x), static_cast<UINT>(pl->Getm_Pos().y - 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x - 1), static_cast<UINT>(pl->Getm_Pos().y - 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pl->Getm_Pos().x + 1), static_cast<UINT>(pl->Getm_Pos().y - 1));
		if (tile->Object3 == OBJECT_ENEMY)
		{
			EnemyBase * base = CHARTERMANAGER->FindEnemy(tile->ImageKey3);
			base->Setm_HpRender(true);
			base->Damage(m_AttackDmg);
			base->HitVoice();
		}
		break;
	}
}

