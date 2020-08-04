#pragma once

#include "GameNode.h"

class PlayerUI : public GameNode
{
	int m_PlayerLife;
	int m_PlayerMaxLife;
	int m_BeatCount;
	bool m_IsPlayerDie;
	int m_Beat;
	bool m_IsInven[5];
	bool m_IsPack;
	int CurrentCoin;
	float m_DelayTime;
	bool m_IsTempo;
	int CurrentDiamond;
public:
	PlayerUI();
	~PlayerUI() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void HpRender();
	void CoinRender();
	void InventoryOpen(int i);
	void InventoryRender();
	void HpBeatOn();
	MAKEGETSET(bool, m_IsPlayerDie);
	MAKEGETSET(int, m_PlayerMaxLife);
	MAKEGETSET(int, m_PlayerLife);
	MAKEGETSET(int, CurrentCoin);
	MAKEGETSET(int, CurrentDiamond);
	MAKEGETSET(bool, m_IsTempo);
};

