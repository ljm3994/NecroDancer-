#pragma once

enum InvenKind
{
	INVEN_EMPTY,
	INVEN_SHOVEL,
	INVEN_WEPON,
	INVEN_ARMOR,
	INVEN_HEAD,
	INVEN_FEET,
	INVEN_POSION,
	INVEN_KEY,
	INVEN_PACK,
	INVEN_HEART
};

class ItemBase
{
protected:
	bool m_SetInventory;
	bool m_IsEffect;
	string m_Key;
	int m_Price;
	InvenKind ItemKind;
	D3DXVECTOR2 m_Pos;
	float m_Scale;
	int m_Count;
public:
	ItemBase();
	~ItemBase();
	virtual HRESULT Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void EffectRender() = 0;
	virtual void SetInventory(bool Set);
	virtual int GetItemDmg() = 0;
	virtual void PutItem(D3DXVECTOR2 pos, float Alpha) = 0;
	virtual void TileItemRender(D3DXVECTOR2 m_Pos, float m_Alpha) = 0;
	virtual bool GetItem(Player * player) = 0;
	MAKEGETSET(D3DXVECTOR2, m_Pos);
	MAKEGETSET(bool, m_SetInventory);
	MAKEGETSET(bool, m_IsEffect);
	MAKEGETSET(int, m_Count);
	MAKEGETSET(int, m_Price);
	MAKEGETSET(InvenKind, ItemKind);
	MAKEGETSET(string, m_Key);
};

