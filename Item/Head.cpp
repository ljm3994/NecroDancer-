#include "stdafx.h"
#include "Head.h"


Head::Head()
{
}


Head::~Head()
{
}

HRESULT Head::Init()
{
	Setm_IsEffect(false);

	return S_OK;
}

void Head::SetHead(int Deffence, bool InvenSet, string Key)
{
	m_Deffence = Deffence;
	Setm_SetInventory(InvenSet);
	this->Key = Key;
	m_IsPutItem = false;
}

void Head::Update()
{
}

void Head::PutItem(D3DXVECTOR2 pos, float Alpha)
{
	ST_TILE * tile = MAPMANAGER->GetAllMap()->GetTile(static_cast<UINT>(pos.x), static_cast<UINT>(pos.y));
	tile->Object2 = OBJECT_ITEM;
	tile->ImagKey2 = m_Key;
}

void Head::InventoryRender(string ImgKey)
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage(ImgKey);
		img->Position(380 + g_ptCam.x, (WINSIZEY - 100) + g_ptCam.y);
		img->Color(0, 0, 0, 1);
		img->Scale(50, 50);
		img->Render();
	}
}

int Head::GetItemDmg()
{
	return m_Deffence;
}
