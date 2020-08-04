#pragma once
#include "GameNode.h"
class MainSceen : public GameNode
{
	Player m_TempPlayer;
	bool m_Pause;
public:
	MainSceen();
	~MainSceen() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

