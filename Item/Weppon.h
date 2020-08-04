#pragma once
#include "ItemBase.h"

class Weppon : public ItemBase
{
protected:
	int m_RangeX;
	int m_RangeY;
	bool m_IsPutItem;
	float m_RotateY;
	float EffectDelay;
	int m_AttackDmg;
	int m_EffectAnimeX;
	int m_EffectAnimeY;
public:
	Weppon();
	~Weppon();
	HRESULT Init() override;
	void SetWeppon(int AttackDmg, bool InvenSet, int RangeX, int RangeY , string Key);
	void Update() override;
	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void WeaponAime();
	void InventoryRender(string ImgKey);
	int GetItemDmg() override;
	virtual void WepponAttack(MOVEDIR dir) = 0;
};

