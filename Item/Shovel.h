#pragma once
#include "ItemBase.h"
class Shovel : public ItemBase
{
protected:
	float m_RotateY;
	float m_EffectDelayTime;
	bool m_MovEffect;
public:
	Shovel();
	~Shovel();
	void InventoryRender(string ImgKey);
	HRESULT Init() override;
	void Update() override;

	void PutItem(D3DXVECTOR2 pos, float Alpha) override;
	void BrokenWall();
	int GetItemDmg() override;
};

