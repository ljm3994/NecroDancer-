#pragma once
#include "EnemyBase.h"
enum JOMBISTATE
{
	ZOMBE_HEAD,
	ZOMBE_ALONE,
	ZOMBE_IDLE
};
class Jombi : public EnemyBase
{
	Jombi * m_Next;
	Jombi * m_Before;
	JOMBISTATE m_state;
	bool m_IsUpdate;
	int m_JomSight;
public:
	Jombi();
	~Jombi(); 
	void JombeInit();
	void release() override;
	void update() override;
	void render() override;
	void HitVoice() override;
	void AttackVoice() override;
	void DeathVoice() override;
	void AttackRender() override;
	void ZomUpdate();
	void HeadUpdate();
	void RayCastUpdate();
	void AstarPath(bool isBeat) override;
	void PosUpdate() override;
	void Move(MOVEDIR dir) override;
	void PutCoin() override;
	bool Die() override;
	MAKEGETSET(Jombi *, m_Next);
	MAKEGETSET(Jombi *, m_Before);
};

