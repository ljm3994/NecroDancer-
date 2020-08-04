#pragma once
#include "EnemyBase.h"
class Skeleton : public EnemyBase
{
	bool m_HitSound;
public:
	Skeleton();
	~Skeleton();
	void release() override;
	void update() override;
	void render() override;
	void HitVoice() override;
	void AttackVoice() override;
	void DeathVoice() override;
	void AttackRender() override;
	void PutCoin() override;
};

