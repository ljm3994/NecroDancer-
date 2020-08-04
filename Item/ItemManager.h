#pragma once
#include "Singleton/SingletonBase.h"
#include "ShovelBasic.h"
#include "Dagger.h"
#include "Coin.h"
#include "BackPack.h"
#include "HeartItem.h"
#include "LetherArmor.h"
#include "KeyItem.h"

class ItemManager : public SingletonBase<ItemManager>
{
protected:
	map<string, ItemBase*> m_Item;
public:
	ItemManager();
	~ItemManager();
	void AddItem(string key, ItemBase * Item);
	void CreateItem();
	ItemBase * FindItem(string Key);
	string FindItem(int Index);
	int GetMaxSize();
};

