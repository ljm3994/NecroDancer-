#pragma once



//디버그,문자집합 매크로
#if defined UNICODE && defined _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#elif defined _MBCS && defined _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif 

//매크로 함수
#define SAFE_DELETE(p) {if(p){delete(p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p) = nullptr;}}
#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = nullptr; } }
#define MAKEGET(Type, Value)	\
	Type Get##Value(){return Value;}
#define MAKESET(Type, Value)	\
	void Set##Value(Type val){Value = val;}
#define MAKEGETSET(Type, Value) \
	MAKEGET(Type, Value) MAKESET(Type, Value)
#define MAKEREFGET(Type, Value)	\
	Type& GetRef##Value(){return Value;}

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include <assert.h>
#include <random>
#include <unordered_map>
#include <set>
#include <thread>
#include <mutex>
#include <future>
using namespace std;

typedef mt19937 RandomGet;

#include <d3d9.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3d11_4.h>
#include <d3dX10.h>
#include <d3dx10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//ImGui
#include "imgui/imgui.h"
#include "imgui/imguiDx11.h"
#pragma comment(lib,"imgui/imgui.lib")
#pragma comment(lib,"Corutine/libboost_coroutine-vc142-mt-x32-1_71.lib")
//Direct Write
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#include <vfw.h>
#pragma comment(lib,"vfw32.lib")

//문자집합 매크로
#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#else
#define _tstring string
#define _tcout cout
#endif

#include "MyFunc.h"
#include "Object/InstanceShader.h"
#include "Object/Model.h"
#include "Object/InstanceImage.h"
#include "./Object/Rect.h"
#include "./Object/Circle.h"
#include "./Object/Shader.h"
#include "./Object/Image.h"

//싱글톤 추가
#include "./Singleton/KeyManager.h"
#define KEYMANAGER KeyManager::GetInstance()
#include "./Singleton/TimeManager.h"
#define TIMEMANAGER TimeManager::GetInstance()
#define g_ETime TimeManager::GetInstance()->GetElapsedTime()
#include "./Singleton/SoundManager.h"
#define SOUNDMANAGER SoundManager::GetInstance()
#include"./Singleton/CameraManager.h"
#define CAMMANAGER CameraManager::GetInstance()
#include"./Singleton/ImageManager.h"
#define IMGMANAGER ImageManager::GetInstance()
#include"./Singleton/WriteManager.h"
#define WRITEMANAGER WriteManager::GetInstance()
#include "Singleton/MapManager.h"
#define MAPMANAGER MapManager::GetInstance()
#include "Singleton/Astar.h"
#define ASTARMANAGER Astar::GetInstance()
#include "Singleton/MapMaker.h"
#define MAPMAKERMANAGER MapMaker::GetInstance()
#include "Singleton/InstanceImageManager.h"
#define InstanceImgManager InstanceImageManager::GetInstance()
#include "Singleton/SceneManager.h"
#define SCENEMANAGER SceneManager::GetInstance()
#include "Singleton/VideoManager.h"
#define VIDEOMANAGER VideoManager::GetInstance()
#include "Beat/BeatDetection.h"
#define BEATMANAGER BeatDetection::GetInstance()
#include "Singleton/ObjectPool.h"
#define OBJECTPOOL(Type) ObjectPool<Type>::GetInstance()
#include "Charter/CharcterManager.h"
#define CHARTERMANAGER CharcterManager::GetInstance()
#include "UI/UIManager.h"
#define UIMANAGER UIManager::GetInstance()
#include "Item/ItemManager.h"
#define ITEMMANAGER ItemManager::GetInstance()
#include "Singleton/GameInfo.h"
#define GAMEMANAGER GameInfo::GetInstance()
#include "Trap/TrapManager.h"
#define TRAPMANAGER TrapManager::GetInstance()

#define WINNAME _T("DX11")
#define WINSTARTX 0
#define WINSTARTY 0
#define WINSIZEX 1200
#define WINSIZEY 800
#define FIXFRAME 600

//전역변수
extern HINSTANCE	g_hInst;
extern HWND			g_hwnd;
extern IDXGISwapChain*				SwapChain;
extern ID3D11Device*				Device;
extern ID3D11DeviceContext*			DeviceContext;
extern ID3D11RenderTargetView*		RTV;
extern D3DXVECTOR2		g_ptMouse;
extern D3DXVECTOR2		g_ptCam;
extern ID3D11Multithread * mutilethread;
extern bool bThread;
extern float XScroll;
extern bool bLoding;
