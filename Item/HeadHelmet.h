#pragma once
#include "Head.h"
class HeadHelmet : public Head
{
public:
	HeadHelmet();
	~HeadHelmet();
	bool GetItem(Player * player) override;
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
};

