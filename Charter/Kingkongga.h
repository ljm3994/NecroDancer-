#pragma once
#include "EnemyBase.h"
class Kingkongga : public EnemyBase
{
	int m_CurrentMosterNum;
	bool m_IsDrum;
	int m_DrumCount;
	bool m_IsAttack;
	int m_PlayerSight;
	int Count;
public:
	Kingkongga();
	~Kingkongga();
	void KingkonggaInit();
	void release() override;
	void update() override;
	void render() override;
	void HitVoice() override;
	void AttackVoice() override;
	void DeathVoice() override;
	void AttackRender() override;
	void PutCoin() override;
	void AstarPath(bool isBeat) override;
	void KingKogaRayUpdate();
	bool Die() override;
	MAKEGETSET(int, m_CurrentMosterNum);
};

