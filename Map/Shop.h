#pragma once

class Shop
{
	int Xpos;
	int Ypos;
	int m_Width;
	int m_Height;
	float m_Volum;
	D3DXVECTOR2 m_ShopKeeperPos;
	vector<string> m_ShopItem;
public:
	Shop();
	~Shop();
	void PlayShopSound();
	void SellItem(ItemBase * Item, Player * pl);
	bool BuyItem(string strKey, Player * pl);
	void Render(float Alpha);
	void PriceRender(D3DXVECTOR2 pos, string srtKey, float Alpha);
	void ShopSfxVolum();
	void Update();
	void Init();
	void CreateShop();
	void ItemCreate();
};

