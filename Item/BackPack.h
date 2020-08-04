#pragma once
class BackPack : public ItemBase
{
public:
	BackPack();
	~BackPack();
	HRESULT Init() override;
	void Update() override;
	void EffectRender() override;
	void Render() override;
	int GetItemDmg() override;
	void PutItem(D3DXVECTOR2 pos, float m_Alpha) override;
	void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha);
	bool GetItem(Player * player) override;
};

