#include "stdafx.h"
#include "BorodSword.h"
#include "Chainmail.h"
#include "Platemail.h"
#include "HeadHelmet.h"
#include "FeetGreaves.h"
#include "Whip.h"
#include "ItemManager.h"


ItemManager::ItemManager()
{
}


ItemManager::~ItemManager()
{
}

void ItemManager::AddItem(string key, ItemBase * Item)
{
	m_Item.insert(make_pair(key, Item));
}

void ItemManager::CreateItem()
{
	ShovelBasic * ShovelItem = OBJECTPOOL(ShovelBasic)->GetObObject();
	ShovelItem->Init();
	ShovelItem->SetInventory(true);
	ShovelItem->SetItemKind(INVEN_SHOVEL);
	ShovelItem->Setm_Key("ShovelBasic");
	ShovelItem->Setm_Price(10);
	AddItem("ShovelBasic", ShovelItem);
	Dagger * DaggerItem = OBJECTPOOL(Dagger)->GetObObject();
	DaggerItem->Init();
	DaggerItem->SetInventory(true);
	DaggerItem->SetItemKind(INVEN_WEPON);
	DaggerItem->SetWeppon(1, true, 1, 0, "Dagger");
	DaggerItem->Setm_Price(20);
	AddItem("Dagger", DaggerItem);
	BorodSword * BroadSword = OBJECTPOOL(BorodSword)->GetObObject();
	BroadSword->Init();
	BroadSword->SetInventory(false);
	BroadSword->SetItemKind(INVEN_WEPON);
	BroadSword->SetWeppon(2, false, 3, 0, "BorodSword");
	BroadSword->Setm_Price(50);
	AddItem("BorodSword", BroadSword);
	Whip * WhipWepon = OBJECTPOOL(Whip)->GetObObject();
	WhipWepon->Init();
	WhipWepon->SetInventory(false);
	WhipWepon->SetItemKind(INVEN_WEPON);
	WhipWepon->SetWeppon(3, false, 3, 0, "Whip");
	WhipWepon->Setm_Price(60);
	AddItem("Whip", WhipWepon);
	for (int i = 0; i < 10; i++)
	{
		Coin * Coin1 = OBJECTPOOL(Coin)->GetObObject();
		Coin1->Setm_CoinCount(i+1);
		Coin1->SetItemKind(INVEN_EMPTY);
		Coin1->Init();
		char coinname[256];
		sprintf_s(coinname, "Coin%d", i + 1);
		AddItem(coinname, Coin1);
	}
	BackPack * Pack = OBJECTPOOL(BackPack)->GetObObject();
	Pack->Init();
	Pack->SetInventory(false);
	Pack->Setm_Key("BackPack");
	Pack->SetItemKind(INVEN_PACK);
	Pack->Setm_Price(30);
	AddItem("BackPack", Pack);
	HeartItem * Hitem = OBJECTPOOL(HeartItem)->GetObObject();
	Hitem->Init();
	Hitem->SetInventory(false);
	Hitem->Setm_Key("HeartItem");
	Hitem->SetItemKind(INVEN_HEART);
	Hitem->Setm_Price(60);
	AddItem("HeartItem", Hitem);
	LetherArmor * Lether = OBJECTPOOL(LetherArmor)->GetObObject();
	Lether->Init();
	Lether->SetArmor(1, false, "LeatherArmor");
	Lether->SetInventory(false);
	Lether->Setm_Key("LeatherArmor");
	Lether->SetItemKind(INVEN_ARMOR);
	Lether->Setm_Price(30);
	AddItem("LeatherArmor", Lether);
	Chainmail * ChainLether = OBJECTPOOL(Chainmail)->GetObObject();
	ChainLether->Init();
	ChainLether->SetArmor(2, false, "Chainmail");
	ChainLether->SetInventory(false);
	ChainLether->Setm_Key("Chainmail");
	ChainLether->SetItemKind(INVEN_ARMOR);
	ChainLether->Setm_Price(50);
	AddItem("Chainmail", ChainLether);
	Platemail * PlateLether = OBJECTPOOL(Platemail)->GetObObject();
	PlateLether->Init();
	PlateLether->SetArmor(3, false, "Platemail");
	PlateLether->SetInventory(false);
	PlateLether->Setm_Key("Platemail");
	PlateLether->SetItemKind(INVEN_ARMOR);
	PlateLether->Setm_Price(70);
	AddItem("Platemail", PlateLether);
	HeadHelmet * Helmet = OBJECTPOOL(HeadHelmet)->GetObObject();
	Helmet->Init();
	Helmet->SetHead(1, false, "Headhelmet");
	Helmet->SetInventory(false);
	Helmet->Setm_Key("Headhelmet");
	Helmet->SetItemKind(INVEN_HEAD);
	Helmet->Setm_Price(100);
	AddItem("Headhelmet", Helmet);
	FeetGreaves * Greaves = OBJECTPOOL(FeetGreaves)->GetObObject();
	Greaves->Init();
	Greaves->SetFeet(1, false, "FeetGreaves");
	Greaves->SetInventory(false);
	Greaves->Setm_Key("FeetGreaves");
	Greaves->SetItemKind(INVEN_FEET);
	Greaves->Setm_Price(100);
	AddItem("FeetGreaves", Greaves);
	KeyItem * m_Key = OBJECTPOOL(KeyItem)->GetObObject();
	m_Key->Init();
	m_Key->SetInventory(false);
	m_Key->Setm_Key("LockKey");
	m_Key->SetItemKind(INVEN_KEY);
	m_Key->Setm_Price(1000);
	AddItem("LockKey", m_Key);
}

ItemBase * ItemManager::FindItem(string Key)
{
	return m_Item[Key];
}

string ItemManager::FindItem(int Index)
{
	map<string, ItemBase*>::iterator it;
	int cnt = 0;
	for (it = m_Item.begin(); it != m_Item.end(); it++, cnt++)
	{
		if (Index == cnt)
		{
			return it->first;
		}
	}

	return "";
}

int ItemManager::GetMaxSize()
{
	return m_Item.size();
}
