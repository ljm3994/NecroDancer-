#include "stdafx.h"
#include "Feet.h"


Feet::Feet()
{
}


Feet::~Feet()
{
}

HRESULT Feet::Init()
{
	Setm_IsEffect(false);

	return S_OK;
}

void Feet::SetFeet(int Deffence, bool InvenSet, string Key)
{
	m_Deffence = Deffence;
	Setm_SetInventory(InvenSet);
	this->Key = Key;
	m_IsPutItem = false;
}

void Feet::Update()
{
}

void Feet::PutItem(D3DXVECTOR2 pos, float Alpha)
{
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pos.x), static_cast<UINT>(pos.y));
	tile->Object2 = OBJECT_ITEM;
	tile->ImagKey2 = m_Key;
}

void Feet::InventoryRender(string ImgKey)
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage(ImgKey);
		img->Position(480 + g_ptCam.x, (WINSIZEY - 100) + g_ptCam.y);
		img->Color(0, 0, 0, 1);
		img->Scale(50, 50);
		img->Render();
	}
}

int Feet::GetItemDmg()
{
	return m_Deffence;
}
