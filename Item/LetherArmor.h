#pragma once
#include "Armor.h"
class LetherArmor : public Armor
{
public:
	LetherArmor();
	~LetherArmor();
	bool GetItem(Player * player) override;
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
};

