#pragma once
#include "GameNode.h"
#include "MiniGame/LoadingSceen.h"

class MainGame :public GameNode
{
public:
	MainGame();
	~MainGame() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

};

