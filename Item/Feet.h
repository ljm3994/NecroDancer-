#pragma once
class Feet : public ItemBase
{
protected:
	string Key;
	bool m_IsPutItem;
	int m_Deffence;
public:
	Feet();
	~Feet();
	HRESULT Init() override;
	void SetFeet(int Deffence, bool InvenSet, string Key);
	void Update() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void InventoryRender(string ImgKey);
	int GetItemDmg() override;
};

