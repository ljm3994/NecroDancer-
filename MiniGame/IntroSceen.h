#pragma once
#include "GameNode.h"
class IntroSceen : public GameNode
{
	int ImageNum;
	float SubTileDelay;
	bool ShowSubTitle;
	bool First;
public:
	IntroSceen();
	~IntroSceen()override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

