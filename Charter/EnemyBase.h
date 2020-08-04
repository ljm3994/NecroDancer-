#pragma once
#include "Charter.h"
class EnemyBase : public Charter
{
protected:
	Room * m_MyRoom;
	string Key;
	float m_Alpha;
	int m_Range;
	bool PathSucced;
	int m_Damage;
	int m_Deffence;
	stack<POINT> m_Path;
	int m_AniCount;
	int m_CurrentFramX;
	int m_CurrentFramY;
	float m_HpShowDelay;
	bool m_isAttackImp;
	bool m_HpRender;
	D3DXVECTOR2 m_AttackPos;
	float AttackRotateZ;
	float AttackDelay;
	bool m_IsAttackAnime;
	bool m_IsPosUpdate;
	float m_DelayTime;
public:
	EnemyBase();
	~EnemyBase();
	virtual HRESULT init() override;
	void SettingEnemy(UINT m_maxHp, UINT m_currentHp, int m_Damage, int m_Deffence);
	void SetPos(D3DXVECTOR2 m_Pos);
	virtual void AstarPath(bool isBeat);
	virtual void PosUpdate();
	void Move(MOVEDIR dir) override;
	void Attack() override;
	void HpRender();
	void Damage(int Attack) override;
	void Deffence(int& Attack) override;
	void TileCheck();
	virtual void AttackRender() = 0;
	virtual void HitVoice() = 0;
	virtual void AttackVoice() = 0;
	virtual void DeathVoice() = 0;
	virtual void PutCoin() = 0;
	bool Die() override;
	MAKEGETSET(bool, m_HpRender);
	MAKEGETSET(D3DXVECTOR2, m_Pos);
	MAKEGETSET(Room *, m_MyRoom);
	MAKEGETSET(string, Key);
	MAKEGETSET(bool, m_Alive);
	MAKEGETSET(float, m_Alpha);
	MAKEGETSET(bool, m_IsPosUpdate);
};

