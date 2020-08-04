#include "stdafx.h"
#include "Armor.h"


Armor::Armor()
{
}


Armor::~Armor()
{
}

HRESULT Armor::Init()
{
	Setm_IsEffect(false);
	m_IsPutItem = false;
	return S_OK;
}

void Armor::SetArmor(int Deffence, bool InvenSet, string Key)
{
	m_Deffence = Deffence;
	Setm_SetInventory(InvenSet);
	this->Key = Key;
	m_IsPutItem = false;
}

void Armor::Update()
{
}

void Armor::PutItem(D3DXVECTOR2 pos, float Alpha)
{
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pos.x), static_cast<UINT>(pos.y));
	tile->Object2 = OBJECT_ITEM;
	tile->ImagKey2 = m_Key;
}


void Armor::InventoryRender(string ImgKey)
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage(ImgKey);
		img->Position(280 + g_ptCam.x, (WINSIZEY - 100) + g_ptCam.y);
		img->Scale(50, 50);
		img->Render();
	}
}

int Armor::GetItemDmg()
{
	return m_Deffence;
}

