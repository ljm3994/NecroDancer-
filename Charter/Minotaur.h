#pragma once
class Minotaur : public EnemyBase
{
public:
	Minotaur();
	~Minotaur();
	void release() override;
	void update() override;
	void render() override;
	void HitVoice() override;
	void AttackVoice() override;
	void DeathVoice() override;
	void AttackRender() override;
	void PutCoin() override;
};

