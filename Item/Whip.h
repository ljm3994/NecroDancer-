#pragma once

class Whip : public Weppon
{
public:
	Whip();
	~Whip();
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
	bool GetItem(Player * player) override;
	void WepponAttack(MOVEDIR dir) override;
};

