#include "stdafx.h"
#include "MainGame.h"
#include "MiniGame/MainSceen.h"
#include "MiniGame/IntroSceen.h"
#include "MiniGame/FileSelect.h"
#include "MiniGame/BossStage.h"
#include "MiniGame/LobbyStage.h"
#include "Map/MapTool.h"
MainGame::MainGame()
{
}


MainGame::~MainGame()
{
	//싱글톤 해제
	KEYMANAGER->ReleaseSingleton();
	TIMEMANAGER->ReleaseSingleton();
	SOUNDMANAGER->ReleaseSingleton();
	CAMMANAGER->ReleaseSingleton();
	IMGMANAGER->DeleteAll();
	IMGMANAGER->ReleaseSingleton();
	WRITEMANAGER->ReleaseSingleton();
	//MAPMANAGER->ReleaseSingleton();
	ASTARMANAGER->ReleaseSingleton();
	MAPMAKERMANAGER->ReleaseSingleton();
	InstanceImgManager->ReleaseSingleton();
	SCENEMANAGER->ReleaseSingleton();
	VIDEOMANAGER->ReleaseSingleton();
	BEATMANAGER->ReleaseSingleton();
	CHARTERMANAGER->ReleaseSingleton();
	UIMANAGER->ReleaseSingleton();
	ITEMMANAGER->ReleaseSingleton();
	GAMEMANAGER->ReleaseSingleton();

}

HRESULT MainGame::init()
{
	//윈도우가 시작되서 지금까지 흐른 시간을 1/1000초 단위로
	//DWORD형을 리턴하는 함수다
	srand(GetTickCount());
	////////////////////////////////////////////////////
	// 로딩 이미지 추가
	IMGMANAGER->AddImage("LoadingImg1", _T("LoadingImg/Loading1.png"), "PS2");
	IMGMANAGER->AddImage("LoadingImg2", _T("LoadingImg/Loading2.png"), "PS2");
	IMGMANAGER->AddImage("ConsolLoading", _T("LoadingImg/console_loading.bmp"),  "PS2");
	IMGMANAGER->AddImage("Continue", _T("LoadingImg/continue.bmp"), "PS2");
	// 인트로 영상 추가
	_TCHAR str[256] = _T("Movie/IntroSub.wmv");
	VIDEOMANAGER->AddMovie("Intro", str, 0, 0, 0, 0);
	// 배경 사운드 추가
	SOUNDMANAGER->addSound("IntroSound", "Movie/intro.ogg", true);
	// 메인 메뉴 사운드 추가
	SOUNDMANAGER->addSound("MainMenu", "Sound/main_menu.ogg", true);

	// 씬 추가
	SCENEMANAGER->AddScene("IntroSceen", new IntroSceen());
	SCENEMANAGER->AddScene("Loading", new LoadingSceen());
	SCENEMANAGER->AddScene("Main", new MainSceen());
	SCENEMANAGER->AddScene("FileSelect", new FileSelect());
	SCENEMANAGER->AddScene("FileSelect", new FileSelect());
	SCENEMANAGER->AddScene("BossStage", new BossStage());
	SCENEMANAGER->AddScene("LobbyStage", new LobbyStage());
	SCENEMANAGER->AddScene("MapTool", new MapTool());

	SCENEMANAGER->ChangeScene("IntroSceen");

	////////////////////////////////////////////////////
	ImGui::Create(g_hwnd, Device, DeviceContext);
	ImGui::StyleColorsDark();
	return S_OK;
}

void MainGame::release()
{
	////////////////////////////////////////////////////

	SCENEMANAGER->ReleaseAll();
	SCENEMANAGER->ReleaseSingleton();
	////////////////////////////////////////////////////
	ImGui::Delete();
}

void MainGame::update()
{
	////////////////////////////////////////////////////

	SCENEMANAGER->CurrentSceneUpdate();

	////////////////////////////////////////////////////
	SOUNDMANAGER->Update();
	ImGui::Update();
}

void MainGame::render()
{
	if (!VIDEOMANAGER->GetPlay())
	{
		D3DXCOLOR background = D3DXCOLOR(0, 0, 0, 1);
		mutilethread->Enter();
		DeviceContext->ClearRenderTargetView(RTV, (float *)background);
		CAMMANAGER->SetBuffers();
		WRITEMANAGER->GetDc()->BeginDraw();
		TIMEMANAGER->render();
		////////////////////////////////////////////////////

		SCENEMANAGER->CurrentSceneRender();
		////////////////////////////////////////////////////

		WRITEMANAGER->GetDc()->EndDraw();
		SwapChain->Present(0, 0);
		mutilethread->Leave();

	}

ImGui::Render();

}
