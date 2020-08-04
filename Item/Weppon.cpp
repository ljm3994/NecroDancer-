#include "stdafx.h"
#include "Weppon.h"


Weppon::Weppon()
{
}


Weppon::~Weppon()
{
}

HRESULT Weppon::Init()
{
	m_EffectAnimeX = 0;
	m_EffectAnimeY = 0;
	EffectDelay = 0.0f;
	Setm_IsEffect(false);
	m_IsPutItem = false;
	return S_OK;
}

void Weppon::SetWeppon(int AttackDmg, bool InvenSet, int RangeX, int RangeY,  string Key)
{
	m_AttackDmg = AttackDmg;
	SetInventory(InvenSet);
	m_RangeX = RangeX;
	m_RangeY = RangeY;
	m_Key = Key;
}

void Weppon::Update()
{
	WeaponAime();
}

void Weppon::PutItem(D3DXVECTOR2 pos, float Alpha)
{
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pos.x), static_cast<UINT>(pos.y));
	tile->Object2 = OBJECT_ITEM;
	tile->ImagKey2 = m_Key;
}


void Weppon::WeaponAime()
{
	if (Getm_IsEffect())
	{
		Player * m_player = CHARTERMANAGER->GetPlayer();
		MOVEDIR dir = m_player->GetDir();

		if (dir != MOVE_NO)
		{
			switch (dir)
			{
			case MOVE_LEFT:
				m_RotateY = PI;
				Setm_Pos(D3DXVECTOR2(m_player->Getm_Pos().x - 1, m_player->Getm_Pos().y));
				break;
			case MOVE_RIGHT:
				m_RotateY = 0;
				Setm_Pos(D3DXVECTOR2(m_player->Getm_Pos().x + 1, m_player->Getm_Pos().y));
				break;
			case MOVE_DOWN:
				m_RotateY = 2 * PI;
				Setm_Pos(D3DXVECTOR2(m_player->Getm_Pos().x, m_player->Getm_Pos().y - 1));
				break;
			case MOVE_UP:
				m_RotateY = PI / 2;
				Setm_Pos(D3DXVECTOR2(m_player->Getm_Pos().x, m_player->Getm_Pos().y + 1));
				break;
			}
		}
	}
}

void Weppon::InventoryRender(string ImgKey)
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage(ImgKey);
		img->Position(180 + g_ptCam.x, (WINSIZEY - 100) + g_ptCam.y);
		img->Color(0, 0, 0, 1);
		img->Scale(70, 70);
		img->Render();
	}
}

int Weppon::GetItemDmg()
{
	return m_AttackDmg;
}
