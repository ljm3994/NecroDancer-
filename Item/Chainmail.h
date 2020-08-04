#pragma once
class Chainmail : public Armor
{
public:
	Chainmail();
	~Chainmail();
	bool GetItem(Player * player) override;
	void Render() override;
	void EffectRender() override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) override;
};

