#pragma once
#include "Feet.h"
class FeetGreaves : public Feet
{
public:
	FeetGreaves();
	~FeetGreaves();
	bool GetItem(Player * player) override;
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
};

