#include "stdafx.h"
#include "FileSelect.h"


FileSelect::FileSelect()
{
}


FileSelect::~FileSelect()
{
}

HRESULT FileSelect::init()
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
	
	m_FileOpen = false;
	return S_OK;
}

void FileSelect::release()
{
}

void FileSelect::update()
{
	if (KEYMANAGER->isKeyDown(VK_SPACE) && !m_FileOpen)
	{
		m_FileOpen = true;
		pfd->Show(0);
	}
	if (KEYMANAGER->isKeyDown(VK_RETURN) && !m_FileOpen)
	{
		if (GAMEMANAGER->Getm_CurrentLevel() >= 0)
		{
			BEATMANAGER->BeatClear();
			SOUNDMANAGER->stop("MainMenu");
			BEATMANAGER->Init("Zone1");
			if (GAMEMANAGER->Getm_CurrentLevel() == 4 && GAMEMANAGER->Getm_Mode() == MODE_STORY)
			{
				SCENEMANAGER->ChangeScene("BossStage");
			}
			else
			{
				SCENEMANAGER->ChangeScene("Main");
			}
		}
		else
		{
			SCENEMANAGER->ChangeScene("LobbyStage");
		}
	}
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
				SOUNDMANAGER->stop("MainMenu");
				SOUNDMANAGER->addSound(fname, strpath, true);
				BEATMANAGER->BeatClear();
				BEATMANAGER->Init(fname);
				if (GAMEMANAGER->Getm_CurrentLevel() == 5)
				{
					SCENEMANAGER->ChangeScene("BossStage");
				}
				else
				{
					SCENEMANAGER->ChangeScene("Main");
				}
				SAFE_DELETE_ARRAY(strpath);
			}
			CoTaskMemFree(path);
		}
		else
		{
			m_FileOpen = false;
		}
		CoUninitialize();
	}
}

void FileSelect::render()
{
	IMGMANAGER->GetImage("Introduce")->Position(WINSIZEX / 2, WINSIZEY / 2);
	IMGMANAGER->GetImage("Introduce")->Scale(WINSIZEX, WINSIZEY);
	IMGMANAGER->GetImage("Introduce")->Render();
}
