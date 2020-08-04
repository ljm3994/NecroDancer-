#include "stdafx.h"
#include "Shovel.h"


Shovel::Shovel()
{
}


Shovel::~Shovel()
{
}

void Shovel::InventoryRender(string ImgKey)
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage(ImgKey);
		img->Color(0, 0, 0, 1);
		img->Position(80 + g_ptCam.x, (WINSIZEY - 110) + g_ptCam.y);
		img->Scale(60, 60);
		img->Render();
	}
}

HRESULT Shovel::Init()
{
	Setm_IsEffect(false);
	m_EffectDelayTime = 0.0f;
	m_MovEffect = false;
	return S_OK;
}

void Shovel::Update()
{
	BrokenWall();
}

void Shovel::PutItem(D3DXVECTOR2 pos, float Alpha)
{
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pos.x), static_cast<UINT>(pos.y));
	tile->Object2 = OBJECT_ITEM;
	tile->ImagKey2 = m_Key;
}

void Shovel::BrokenWall()
{
		if (Getm_IsEffect() && !m_MovEffect)
		{
			m_MovEffect = true;
			Player * m_player = CHARTERMANAGER->GetPlayer();
			MOVEDIR dir = m_player->GetDir();
			D3DXVECTOR2 Pos = m_player->Getm_Pos();
			if (dir != MOVE_NO)
			{
				switch (dir)
				{
				case MOVE_LEFT:
					m_RotateY = PI;
					Pos = D3DXVECTOR2(m_player->Getm_Pos().x - 1, m_player->Getm_Pos().y);
				
					break;
				case MOVE_RIGHT:
					m_RotateY = 0;
					Pos = D3DXVECTOR2(m_player->Getm_Pos().x + 1, m_player->Getm_Pos().y);
					break;
				case MOVE_DOWN:
					m_RotateY = 2 * PI;
					Pos = D3DXVECTOR2(m_player->Getm_Pos().x, m_player->Getm_Pos().y - 1);
					break;
				case MOVE_UP:
					m_RotateY = PI / 2;
					Pos = D3DXVECTOR2(m_player->Getm_Pos().x, m_player->Getm_Pos().y + 1);

					break;
				}

				ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(Pos.x), static_cast<UINT>(Pos.y));
				Setm_Pos(Pos);
				if (tile->Object == TileObject::OBJECT_BROKENWALL)
				{
					SOUNDMANAGER->play("WallBrick", GAMEMANAGER->GetsfxVolum());
					tile->Object = TileObject::OBJECT_EMPTY;
					tile->ImageKey = "";
					tile->ImagKey2 = "";
					tile->Object2 = TileObject::OBJECT_EMPTY;
					tile->Property = TileProperty::PASS;
				}
				else if (tile->Object == TileObject::OBJECT_TWOBROKENWALL)
				{
					SOUNDMANAGER->play("WallBrick", GAMEMANAGER->GetsfxVolum());
					tile->Object = TileObject::OBJECT_BROKENWALL;
					tile->Property = TileProperty::NONPASS;
				}
				else
				{
					SOUNDMANAGER->play("WallFail", GAMEMANAGER->GetsfxVolum());
				}
			}
		}
}

int Shovel::GetItemDmg()
{
	return 0;
}
