#pragma once
class Platemail : public Armor
{
public:
	Platemail();
	~Platemail();
	bool GetItem(Player * player) override;
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
};

