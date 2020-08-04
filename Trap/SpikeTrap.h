#pragma once
#include "TrapBase.h"
class SpikeTrap : public TrapBase
{
public:
	SpikeTrap();
	~SpikeTrap();
	void Update() override;
	void Render(float Alpha) override;
	void TrapDown(Charter * m_chr) override;
};

