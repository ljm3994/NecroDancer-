#pragma once
#include "ItemBase.h"

class Coin : public ItemBase
{
	int m_CoinCount;
public:
	Coin();
	~Coin();
	HRESULT Init() override;
	void Update() override;
	void EffectRender() override;
	void Render() override;
	int GetItemDmg() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha);
	bool GetItem(Player * player) override;
	MAKEGETSET(int, m_CoinCount);
};

