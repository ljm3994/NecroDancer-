#pragma once
#include "ItemBase.h"
class Armor : public ItemBase
{
protected:
	string Key;
	bool m_IsPutItem;
	int m_Deffence;
public:
	Armor();
	~Armor();
	HRESULT Init() override;
	void SetArmor(int Deffence, bool InvenSet, string Key);
	void Update() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void InventoryRender(string ImgKey);
	int GetItemDmg() override;
};

