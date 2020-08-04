#pragma once
#include "ItemBase.h"
class HeartItem : public ItemBase
{
public:
	HeartItem();
	~HeartItem();
	HRESULT Init() override;
	void Update() override;
	void EffectRender() override;
	void Render() override;
	int GetItemDmg() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha);
	bool GetItem(Player * player) override;
};

