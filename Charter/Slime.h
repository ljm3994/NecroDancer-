#pragma once
class Slime : public EnemyBase
{
public:
	Slime();
	~Slime();
	void release() override;
	void update() override;
	void render() override;
	void HitVoice() override;
	void AttackVoice() override;
	void DeathVoice() override;
	void AttackRender() override;
	void PutCoin() override;
};

