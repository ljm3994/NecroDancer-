#include "stdafx.h"
#include "BackPack.h"


BackPack::BackPack()
{
}


BackPack::~BackPack()
{
}

HRESULT BackPack::Init()
{
	Setm_IsEffect(false);
	return S_OK;
}

void BackPack::Update()
{
}

void BackPack::EffectRender()
{
}

void BackPack::Render()
{
	if (Getm_SetInventory())
	{
		Image * img = IMGMANAGER->GetImage("BackPack");
		img->Position(80 + g_ptCam.x, (WINSIZEY - 200) + g_ptCam.y);
		img->Color(0, 0, 0, 1.0f);
		img->Scale(60, 60);
		img->Render();
	}
}

int BackPack::GetItemDmg()
{
	return 0;
}

void BackPack::PutItem(D3DXVECTOR2 pos, float m_Alpha)
{

}

void BackPack::TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha)
{
	Image * img = IMGMANAGER->GetImage("BackPack");
	img->Color(0, 0, 0, m_Alpha);
	img->Position(m_Pos.x, m_Pos.y);
	img->Scale(50, 50);
	img->GetRefCurrentFrameX() = 0;
	img->GetRefCurrentFrameY() = 0;
	img->Render();
}

bool BackPack::GetItem(Player * player)
{
	Setm_SetInventory(true);
	for (int i = 0; i < 5; i++)
	{
		UIMANAGER->Getm_PlayerUI()->InventoryOpen(i);
	}
	return true;
}
