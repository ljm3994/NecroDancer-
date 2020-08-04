#include "stdafx.h"
#include "IntroSceen.h"


IntroSceen::IntroSceen()
{
}


IntroSceen::~IntroSceen()
{
}

HRESULT IntroSceen::init()
{
	VIDEOMANAGER->Play("Intro");
	SOUNDMANAGER->play("IntroSound", GAMEMANAGER->GetMusicVolum());
	ImageNum = 0;
	SubTileDelay = 0.0f;
	ShowSubTitle = false;
	return S_OK;
}

void IntroSceen::release()
{
}

void IntroSceen::update()
{
	if (VIDEOMANAGER->MovieCanPlay())
	{
		if (KEYMANAGER->isKeyDown(VK_SPACE))
		{
			VIDEOMANAGER->Stop();
		}
	}
	else
	{
		SOUNDMANAGER->stop("IntroSound");
		SCENEMANAGER->ChangeScene("Loading");
	}
}

void IntroSceen::render()
{
	
}
