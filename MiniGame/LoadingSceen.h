#pragma once
#include <thread>

#include "GameNode.h"


class LoadingSceen : public GameNode
{
	int Level;
	Image * LoadingImage;
	D3DXVECTOR2 m_Pos;
	unique_lock<mutex> Lock;
	mutex gSynch2;
	int LoadingNum;
	float ImageDelayTime;
	promise<void> p;
	future<void> data;
	future_status status;
	string ImageName;
	int m_DivNum;
	int MaxLoadingNum;
	bool bFirst;

public:
	LoadingSceen();
	~LoadingSceen() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void MapLoadData();
	void GameDataLoad();
};
