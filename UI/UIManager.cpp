#include "stdafx.h"
#include "UIManager.h"


UIManager::UIManager()
{
}


UIManager::~UIManager()
{
	if (m_BeatUI)
	{
		m_BeatUI->release();
		SAFE_DELETE(m_BeatUI);
	}

	if (m_PlayerUI)
	{
		m_PlayerUI->release();
		SAFE_DELETE(m_PlayerUI);
	}

	if (m_DieUI)
	{
		m_DieUI->release();
		SAFE_DELETE(m_DieUI);
	}

	if (m_OptionUI)
	{
		m_OptionUI->release();
		SAFE_DELETE(m_OptionUI);
	}
}

void UIManager::Init()
{
	if (m_BeatUI)
	{
		m_BeatUI->release();
		SAFE_DELETE(m_BeatUI);
	}
	if (m_DieUI)
	{
		m_DieUI->release();
		SAFE_DELETE(m_DieUI);
	}
	if (m_OptionUI)
	{
		m_OptionUI->release();
		SAFE_DELETE(m_OptionUI);
	}
	m_BeatUI = new WinUI();
	m_BeatUI->init();
	m_DieUI = new DieUI();
	m_DieUI->init();
	m_OptionUI = new OptionUI();
	m_OptionUI->init();
}

void UIManager::Update()
{
	if (!m_OptionUI->Getm_Pause())
	{
		if (CHARTERMANAGER->Getm_CreateCharter())
		{
			m_BeatUI->update();
			m_PlayerUI->update();
		}
		if (!CHARTERMANAGER->GetPlayer()->Getm_Alive())
		{
			m_DieUI->update();
		}
	}
	else
	{
		m_OptionUI->update();
	}
}

void UIManager::Render()
{
	if (!m_OptionUI->Getm_Pause())
	{
		CHARTERMANAGER->Render();

		if (CHARTERMANAGER->Getm_CreateCharter())
		{
			m_BeatUI->render();
			m_PlayerUI->render();
		}
		if (!CHARTERMANAGER->GetPlayer()->Getm_Alive())
		{
			m_DieUI->render();
		}
	}
	else
	{
		m_OptionUI->render();
	}
}

void UIManager::PlayerUIinit()
{
	m_PlayerUI = new PlayerUI();
	m_PlayerUI->init();
}
