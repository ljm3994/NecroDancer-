#pragma once
#include "Weppon.h"
class Dagger : public Weppon
{
public:
	Dagger();
	~Dagger();
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
	bool GetItem(Player * player) override;
	void WepponAttack(MOVEDIR dir) override;
};

