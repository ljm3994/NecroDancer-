#pragma once
#include "Singleton/SingletonBase.h"
#include "TrapBase.h"

class TrapManager : public SingletonBase<TrapManager>
{
	map<string,TrapBase*> Traps;
public:
	TrapManager();
	~TrapManager();
	string AddTrap(TrapBase * base);
	void CreateTrap();
	TrapBase* FindTrap(string Index);
};

