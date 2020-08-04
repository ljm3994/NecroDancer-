#pragma once
#include "TrapBase.h"
class Box : public TrapBase
{
public:
	Box();
	~Box();
	void Update() override;
	void Render(float Alpha) override;
	void TrapDown(Charter * m_chr) override;
};

