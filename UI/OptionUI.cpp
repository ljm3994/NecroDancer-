#include "stdafx.h"
#include "OptionUI.h"
#include "Beat/FFTDetector.h"
OptionUI::OptionUI()
{
	PlayerKeyVec.emplace_back("Player");
	PlayerKeyVec.emplace_back("Jp1");
	PlayerKeyVec.emplace_back("Jp2");
	PlayerKeyVec.emplace_back("Jp3");
	PlayerKeyVec.emplace_back("Jp4");
	PlayerKeyVec.emplace_back("Jp5");
	PlayerKeyVec.emplace_back("Jp6");
	PlayerKeyVec.emplace_back("Jp7");
	PlayerKeyVec.emplace_back("Jp8");
	PlayerKeyVec.emplace_back("Jp9");
	PlayerKeyVec.emplace_back("Jp10");
	PlayerKeyVec.emplace_back("Jp11");
}


OptionUI::~OptionUI()
{
}

HRESULT OptionUI::init()
{
	m_SelectNum = 0;
	m_AudioOption = false;
	m_SkinChangeOption = false;
	m_Pause = false;
	m_LobbyGo = false;
	m_QuickRestart = false;
	m_FFTOption = false;
	Detector = new FFTDetector();
	if (GAMEMANAGER->Getm_CurrentLevel() == -1)
	{
		m_MaxSelectNum = 2;
	}
	else
	{
		m_MaxSelectNum = 4;
	}

	m_OptionRender = false;
	for (int i = 0; i < PlayerKeyVec.size(); i++)
	{
		if (GAMEMANAGER->Getm_SkinName() == PlayerKeyVec[i])
		{
			m_SkinNum = i;
			break;
		}
	}
	return S_OK;
}

void OptionUI::release()
{
	PlayerKeyVec.clear();
	PlayerKeyVec.shrink_to_fit();
}

void OptionUI::update()
{
	if (!m_FFTOption)
	{
		if (KEYMANAGER->isKeyDown(VK_UP))
		{
			SOUNDMANAGER->play("UISelectUp", GAMEMANAGER->GetsfxVolum());
			m_SelectNum--;

			if (m_SelectNum < 0)
			{
				m_SelectNum = m_MaxSelectNum;
			}
		}
		else if (KEYMANAGER->isKeyDown(VK_DOWN))
		{
			SOUNDMANAGER->play("UISelectDown", GAMEMANAGER->GetsfxVolum());
			m_SelectNum++;

			if (m_SelectNum > m_MaxSelectNum)
			{
				m_SelectNum = 0;
			}
		}
	}
	if (KEYMANAGER->isKeyDown(VK_F1))
	{
		CoInitialize(0);
		COMDLG_FILTERSPEC c_rgSaveTypes[1];
		c_rgSaveTypes[0].pszName = L"MP3 File";
		c_rgSaveTypes[0].pszSpec = L"*.mp3";
		//c_rgSaveTypes[1].pszName = L"WAV File";
		//c_rgSaveTypes[1].pszSpec = L"*.wav";
		pfd = nullptr;
		HRESULT hr = pfd.CoCreateInstance(CLSID_FileOpenDialog, NULL,
			CLSCTX_INPROC_SERVER);
		pfd->SetTitle(L"음원 파일을 선택해주세요");
		pfd->SetOptions(FOS_OVERWRITEPROMPT | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
		pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
		pfd->SetFileTypeIndex(2);
		m_FFTOption = true;
		m_OptionRender = false;
		m_SkinChangeOption = false;
		m_AudioOption = false;
		m_FileOpen = false;
		m_FFTStart = false;
	}
	else if (KEYMANAGER->isKeyDown(VK_RETURN))
	{
		SOUNDMANAGER->play("UISelectStart", GAMEMANAGER->GetsfxVolum());

		if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			if (m_SelectNum == 0)
			{
				m_Pause = false;
				SOUNDMANAGER->resume(BEATMANAGER->GetKey());
				SOUNDMANAGER->SoundVolumChange(BEATMANAGER->GetKey(), GAMEMANAGER->GetMusicVolum());
			}
			else if (m_SelectNum == 1)
			{
				m_OptionRender = true;
				m_SelectNum = 0;
				m_MaxSelectNum = 2;
			}
			else if (m_SelectNum == 2 && GAMEMANAGER->Getm_CurrentLevel() > -1)
			{
				m_QuickRestart = true;
				m_Pause = false;
			}
			else if (m_SelectNum == 3 && GAMEMANAGER->Getm_CurrentLevel() > -1)
			{
				m_LobbyGo = true;
				m_Pause = false;
			}
			else if (m_SelectNum == m_MaxSelectNum)
			{
				PostQuitMessage(0);
			}
		}
		else if (!m_AudioOption && !m_SkinChangeOption && m_OptionRender && !m_FFTOption)
		{
			if (m_SelectNum == 0)
			{
				m_OptionRender = false;
				m_AudioOption = true;
				m_SelectNum = 0;
				m_MaxSelectNum = 2;
				m_MusicVolum = GAMEMANAGER->GetMusicVolum();
				m_SfxVolum = GAMEMANAGER->GetsfxVolum();
				m_SelectMusicVol = m_MusicVolum;
				m_SelectsfxVol = m_SfxVolum;
			}
			else if (m_SelectNum == 1)
			{
				m_OptionRender = false;
				m_MaxSelectNum = 1;
				m_SelectNum = 0;
				m_SkinChangeOption = true;
			}
			else
			{
				m_OptionRender = false;
				m_SelectNum = 0;
				if (GAMEMANAGER->Getm_CurrentLevel() == -1)
				{
					m_MaxSelectNum = 2;
				}
				else
				{
					m_MaxSelectNum = 4;
				}
			}
		}
		else if (m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			if (m_SelectNum == 2)
			{
				GAMEMANAGER->SetMusicVolum(m_SelectMusicVol);
				GAMEMANAGER->SetsfxVolum(m_SelectsfxVol);
				m_AudioOption = false;
				m_OptionRender = true;
				m_SelectNum = 0;
				m_MaxSelectNum = 2;
			}
		}
		else if (!m_AudioOption && m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			if (m_SelectNum == 1)
			{
				m_SkinChangeOption = false;
				m_OptionRender = true;
				m_SelectNum = 0;
				m_MaxSelectNum = 2;
				GAMEMANAGER->Setm_SkinName(PlayerKeyVec[m_SkinNum]);
			}
		}
		else if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && m_FFTOption)
		{
			if (Path != "")
			{
				Detector->BeatDetectionInit(Path);
				m_FFTStart = true;
				SOUNDMANAGER->play(Path);
			}
		}
	}
	else if (KEYMANAGER->isKeyDown(VK_SPACE) && !m_FileOpen)
	{
		if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && m_FFTOption)
		{
			m_FileOpen = true;
			Path = "";
			m_FFTStart = false;
			pfd->Show(0);
		}
	}
	else if (KEYMANAGER->isKeyDown(VK_ESCAPE))
	{
		SOUNDMANAGER->play("UISelectBack", GAMEMANAGER->GetsfxVolum());

		if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			m_Pause = false;
			SOUNDMANAGER->resume(BEATMANAGER->GetKey());
			SOUNDMANAGER->SoundVolumChange(BEATMANAGER->GetKey(), GAMEMANAGER->GetMusicVolum());
		}
		else if (!m_AudioOption && !m_SkinChangeOption && m_OptionRender && !m_FFTOption)
		{
			m_OptionRender = false;
			m_SelectNum = 0;
			if (GAMEMANAGER->Getm_CurrentLevel() == -1)
			{
				m_MaxSelectNum = 2;
			}
			else
			{
				m_MaxSelectNum = 4;
			}

		}
		else if (m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			m_AudioOption = false;
			m_OptionRender = true;
			m_SelectNum = 0;
			m_MaxSelectNum = 2;
			GAMEMANAGER->SetMusicVolum(m_MusicVolum);
			GAMEMANAGER->SetsfxVolum(m_SfxVolum);
		}
		else if (!m_AudioOption && m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			m_SkinChangeOption = false;
			m_OptionRender = true;
			m_SelectNum = 0;
			m_MaxSelectNum = 2;
			for (int i = 0; i < PlayerKeyVec.size(); i++)
			{
				if (GAMEMANAGER->Getm_SkinName() == PlayerKeyVec[i])
				{
					m_SkinNum = i;
					break;
				}
			}
		}
		else if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && m_FFTOption)
		{
			m_FFTOption = false;
		}
	}
	else if (KEYMANAGER->isKeyDown(VK_LEFT))
	{
		if (m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			SOUNDMANAGER->play("UISelectToggle", GAMEMANAGER->GetsfxVolum());
			if (m_SelectNum == 0)
			{
				if (m_SelectMusicVol > 0.0f)
				{
					m_SelectMusicVol -= 0.1f;
				}
			}
			else if (m_SelectNum == 1)
			{
				if (m_SelectsfxVol > 0.0f)
				{
					m_SelectsfxVol -= 0.1f;
				}
			}
			GAMEMANAGER->SetMusicVolum(m_SelectMusicVol);
			GAMEMANAGER->SetsfxVolum(m_SelectsfxVol);
		}
		else if (!m_AudioOption && m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			m_SkinNum--;

			if (m_SkinNum < 0)
			{
				m_SkinNum = PlayerKeyVec.size() - 1;
			}
		}
	}
	else if (KEYMANAGER->isKeyDown(VK_RIGHT))
	{
		if (m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			SOUNDMANAGER->play("UISelectToggle", GAMEMANAGER->GetsfxVolum());
			if (m_SelectNum == 0)
			{
				if (m_SelectMusicVol < 1.0f)
				{
					m_SelectMusicVol += 0.1f;
				}
				else
				{
					m_SelectMusicVol = 1.0f;
				}
			}
			else if (m_SelectNum == 1)
			{
				if (m_SelectsfxVol < 1.0f)
				{
					m_SelectsfxVol += 0.1f;
				}
				else
				{
					m_SelectsfxVol = 1.0f;
				}
			}
			GAMEMANAGER->SetMusicVolum(m_SelectMusicVol);
			GAMEMANAGER->SetsfxVolum(m_SelectsfxVol);
		}
		else if (!m_AudioOption && m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
		{
			m_SkinNum++;

			if (m_SkinNum > PlayerKeyVec.size() - 1)
			{
				m_SkinNum = 0;
			}
		}
	}

	if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && m_FFTOption)
	{
		if (m_FileOpen)
		{
			CComPtr<IShellItem> items;
			if (pfd->GetResult(&items) == S_OK)
			{
				wchar_t * path;
				size_t pReturnValue = 0;
				char Drive[256];
				char dir[256];
				char fname[256];
				char ext[256];
				HRESULT hr = items->GetDisplayName(SIGDN_FILESYSPATH, &path);

				if (SUCCEEDED(hr))
				{
					setlocale(LC_ALL, "");
					char * strpath = new char[255];
					wcstombs_s(&pReturnValue, strpath, 255, path, 255);
					_splitpath_s(strpath, Drive, 256, dir, 256, fname, 256, ext, 256);
					Path = fname;
					SOUNDMANAGER->addSound(fname, strpath, true);
					SAFE_DELETE_ARRAY(strpath);
					m_FileOpen = false;
				}
				CoTaskMemFree(path);
			}
			else
			{
				m_FileOpen = false;
			}
			CoUninitialize();
		}
		if (m_FFTStart)
		{
			//Detector->Update();
		}
	}
}

void OptionUI::render()
{
	// 옵션 틀 렌더 부분
	Image * img = IMGMANAGER->GetImage("PopupBlack");
	img->Scale(WINSIZEX, WINSIZEY);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY / 2) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img = IMGMANAGER->GetImage("BorderHorizontal");
	img->Scale(WINSIZEX - 350, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img->Position((WINSIZEX / 2) + g_ptCam.x, 112.5f + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img = IMGMANAGER->GetImage("BorderVertical");
	img->Scale(50, WINSIZEY - 350);
	img->Position(150 + g_ptCam.x, (WINSIZEY / 2) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img->Position((WINSIZEX - 112.5f) + g_ptCam.x, (WINSIZEY / 2) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img = IMGMANAGER->GetImage("BorderConer1");
	img->Scale(50, 50);
	img->Position(150 + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img = IMGMANAGER->GetImage("BorderConer2");
	img->Scale(50, 50);
	img->Position((WINSIZEX - 150) + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img = IMGMANAGER->GetImage("BorderConer3");
	img->Scale(50, 50);
	img->Position((WINSIZEX - 150) + g_ptCam.x, 150 + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
	img = IMGMANAGER->GetImage("BorderConer4");
	img->Scale(50, 50);
	img->Position(150 + g_ptCam.x, 150 + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	// 메인 옵션 렌더
	if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
	{
		PauseOptionRender();
	}
	// 옵션 창 렌더
	else if (!m_AudioOption && !m_SkinChangeOption && m_OptionRender && !m_FFTOption)
	{
		OptionRender();
	}
	// 오디오 옵션 창 렌더
	else if (m_AudioOption && !m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
	{
		AudioOptionRender();
	}
	// 스킨 변경 창 렌더
	else if (!m_AudioOption && m_SkinChangeOption && !m_OptionRender && !m_FFTOption)
	{
		SkinChangeOptionRender();
	}
	//FFT 창 렌더
	else if (!m_AudioOption && !m_SkinChangeOption && !m_OptionRender && m_FFTOption)
	{
		FFTOptionRender();
	}
}

void OptionUI::PauseOptionRender()
{
	float Position = 200.0f;
	if (GAMEMANAGER->Getm_CurrentLevel() == -1)
	{
		Position = 200.0f;
	}
	else
	{
		Position = 100.0f;
	}
	Image * img = IMGMANAGER->GetImage("PopupBlack");
	img->Scale(500, 100);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage("OptionPause");
	img->Scale(300, 70);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 125) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if(m_SelectNum == 0)
		img = IMGMANAGER->GetImage("GameContinueSelect");
	else
		img = IMGMANAGER->GetImage("GameContinue");

	img->Scale(150, 50);
	Position += 100.0f;
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - Position) + g_ptCam.y);

	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if(m_SelectNum == 1)
		img = IMGMANAGER->GetImage("OptionButtonSelect");
	else
		img = IMGMANAGER->GetImage("OptionButton");
	Position += 100.0f;
	img->Scale(130, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - Position) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (GAMEMANAGER->Getm_CurrentLevel() > -1)
	{
		if (m_SelectNum == 2)
			img = IMGMANAGER->GetImage("OptionQuickStartSelect");
		else
			img = IMGMANAGER->GetImage("OptionQuickStart");
		Position += 100.0f;
		img->Scale(130, 50);
		img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - Position) + g_ptCam.y);
		img->Color(0, 0, 0, 1.0f);
		img->Render();

		if (m_SelectNum == 3)
			img = IMGMANAGER->GetImage("OptionLobbySelect");
		else
			img = IMGMANAGER->GetImage("OptionLobby");
		Position += 100.0f;
		img->Scale(130, 50);
		img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - Position) + g_ptCam.y);
		img->Color(0, 0, 0, 1.0f);
		img->Render();
	}

	if (m_SelectNum == m_MaxSelectNum)
		img = IMGMANAGER->GetImage("OptionExitSelect");
	else
		img = IMGMANAGER->GetImage("OptionExit");
	Position += 100.0f;
	img->Scale(150, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - Position) + g_ptCam.y);

	img->Color(0, 0, 0, 1.0f);
	img->Render();
}

void OptionUI::OptionRender()
{
	Image * img = IMGMANAGER->GetImage("PopupBlack");
	img->Scale(500, 100);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage("OptionTitle");
	img->Scale(200, 70);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 125) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 0)
		img = IMGMANAGER->GetImage("AudioOptionSelect");
	else
		img = IMGMANAGER->GetImage("AudioOption");

	img->Scale(150, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 300) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 1)
		img = IMGMANAGER->GetImage("SkinChangeOptionSelect");
	else
		img = IMGMANAGER->GetImage("SkinChangeOption");

	img->Scale(150, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 400) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 2)
		img = IMGMANAGER->GetImage("OptionOkSelect");
	else
		img = IMGMANAGER->GetImage("OptionOk");

	img->Scale(100, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 600) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
}

void OptionUI::AudioOptionRender()
{
	Image * img = IMGMANAGER->GetImage("PopupBlack");
	img->Scale(500, 100);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage("AudioOptionTitle");
	img->Scale(200, 70);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 125) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 0)
		img = IMGMANAGER->GetImage("MusicVolumOptionSelect");
	else
		img = IMGMANAGER->GetImage("MusicVolumOption");

	img->Scale(150, 50);
	img->Position(250 + g_ptCam.x, (WINSIZEY - 300) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage("Calibration");
	img->Scale(WINSIZEX - 500, 20);
	img->Position(((WINSIZEX / 2) + 100) + g_ptCam.x, (WINSIZEY - 300) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectMusicVol <= 0.0f)
	{
		m_SelectMusicVol = 0.0f;
	}

	img = IMGMANAGER->GetImage("PlayerCoin");
	img->Scale(50, 50);
	img->Position((((WINSIZEX - 525) * m_SelectMusicVol) + 360) + g_ptCam.x, (WINSIZEY - 300) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 1)
		img = IMGMANAGER->GetImage("SFXVolumOptionSelect");
	else
		img = IMGMANAGER->GetImage("SFXVolumOption");

	img->Scale(150, 50);
	img->Position(250 + g_ptCam.x, (WINSIZEY - 400) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage("Calibration");
	img->Scale(WINSIZEX - 500, 20);
	img->Position(((WINSIZEX / 2) + 100) + g_ptCam.x, (WINSIZEY - 400) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectsfxVol <= 0.0f)
	{
		m_SelectsfxVol = 0.0f;
	}

	img = IMGMANAGER->GetImage("PlayerCoin");
	img->Scale(50, 50);
	img->Position((((WINSIZEX - 525) * m_SelectsfxVol) + 360) + g_ptCam.x, (WINSIZEY - 400) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 2)
		img = IMGMANAGER->GetImage("OptionOkSelect");
	else
		img = IMGMANAGER->GetImage("OptionOk");

	img->Scale(100, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 600) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
}

void OptionUI::SkinChangeOptionRender()
{
	Image * img = IMGMANAGER->GetImage("PopupBlack");
	img->Scale(500, 100);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 150) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage("SkinChangeOptionTitle");
	img->Scale(200, 70);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 125) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 0)
		img = IMGMANAGER->GetImage("CharterEditorSelect");
	else
		img = IMGMANAGER->GetImage("CharterEditor");

	img->Scale(150, 50);
	img->Position(((WINSIZEX / 2) - 100) + g_ptCam.x, (WINSIZEY - 400) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage(PlayerKeyVec[m_SkinNum] + "_Body");
	img->Scale(50, 50);
	img->Position(((WINSIZEX / 2) + 100) + g_ptCam.x, (WINSIZEY - 400) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	img = IMGMANAGER->GetImage(PlayerKeyVec[m_SkinNum] + "_Head");
	img->Scale(50, 50);
	img->Position(((WINSIZEX / 2) + 100) + g_ptCam.x, (WINSIZEY - 398) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();

	if (m_SelectNum == 1)
		img = IMGMANAGER->GetImage("OptionOkSelect");
	else
		img = IMGMANAGER->GetImage("OptionOk");

	img->Scale(100, 50);
	img->Position((WINSIZEX / 2) + g_ptCam.x, (WINSIZEY - 600) + g_ptCam.y);
	img->Color(0, 0, 0, 1.0f);
	img->Render();
}

void OptionUI::FFTOptionRender()
{
	int MaxBar = min(64 * 2, 8192);
	float Bpm = 0;
	float interval = 0;

	if (m_FFTStart)
	{
		Detector->RenderFFTVisual({ WINSIZEX * 0.2f, WINSIZEY / 2 }, MaxBar, { WINSIZEX * 0.5, 200 }, 100);
	}
	
}
