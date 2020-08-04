#pragma once
#include "ItemBase.h"
class KeyItem : public ItemBase
{

public:
	KeyItem();
	~KeyItem();
	HRESULT Init() override;
	void Update() override;
	void Render() override;
	void EffectRender() override;
	int GetItemDmg() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
	bool GetItem(Player * player) override;
};

