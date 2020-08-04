#pragma once
#include "GameNode.h"
class LobbyStage : public GameNode
{
	Player * m_Player;
	bool BoostOptionRender;
	int SelectNum;
	bool m_Pause;
	bool m_BoostKeyDown;
public:
	LobbyStage();
	~LobbyStage() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void LobbyRayCast(int xpos, int ypos);
};

