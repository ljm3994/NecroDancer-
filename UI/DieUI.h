#pragma once
#include "GameNode.h"
class DieUI : public GameNode
{
	bool m_RestartButtonOn;
	int m_Select;
	int MaxSelect;
public:
	DieUI();
	~DieUI() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void CoinRender();
};

