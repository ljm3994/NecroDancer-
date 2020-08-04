#pragma once
#include "GameNode.h"
class BossStage : public GameNode
{
	Player * m_Player;
	bool m_BossEnter;
	bool m_ScreenOn;
	bool m_ScreenOut;
	float m_LineDelay;
	bool FirstSoundOn;
	float m_ScreenDelay;
	bool m_IsFirst;
	D3DXVECTOR2 ScreenTop;
	D3DXVECTOR2 ScreenMid;
	D3DXVECTOR2 ScreenBot;
	D3DXVECTOR2 NamePos;
	float m_ScreenAlpha;
public:
	BossStage();
	~BossStage()override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void LightChange();
	void ScreenRender();
	void ScreenUpdate();
	void BossLineRender();
	bool ScreenTopUpdate(int Dirvec);
	bool ScreenMidUpdate(int Dirvec);
	bool ScreenBottomUpdate(int Dirvec);
};

