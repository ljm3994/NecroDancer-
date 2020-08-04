#pragma once
#include "Singleton/SingletonBase.h"
#include "WinUI.h"
#include "PlayerUI.h"
#include "DieUI.h"
#include "OptionUI.h"
class UIManager : public SingletonBase<UIManager>
{
	WinUI * m_BeatUI;
	PlayerUI * m_PlayerUI;
	DieUI * m_DieUI;
	OptionUI * m_OptionUI;
public:
	UIManager();
	~UIManager();
	void Init();
	void Update();
	void Render();
	void PlayerUIinit();
	MAKEGET(WinUI *, m_BeatUI);
	MAKEGET(PlayerUI *, m_PlayerUI);
	MAKEGET(OptionUI *, m_OptionUI);
};


