#pragma once
#include "Shovel.h"

class ShovelBasic : public Shovel
{
public:
	ShovelBasic();
	~ShovelBasic();
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
	bool GetItem(Player * player) override;
};

