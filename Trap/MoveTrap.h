#pragma once
#include "TrapBase.h"

class MoveTrap : public TrapBase
{
	float Rotate;
	MOVEDIR m_Dir;
public:
	MoveTrap();
	~MoveTrap();
	void SettingMove(MOVEDIR Dir);
	void Update() override;
	void Render(float Alpha) override;
	void TrapDown(Charter * m_chr) override;
};

