#pragma once
class Head : public ItemBase
{
protected:
	string Key;
	bool m_IsPutItem;
	int m_Deffence;
public:
	Head();
	~Head();
	HRESULT Init() override;
	void SetHead(int Deffence, bool InvenSet, string Key);
	void Update() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void InventoryRender(string ImgKey);
	int GetItemDmg() override;
};

