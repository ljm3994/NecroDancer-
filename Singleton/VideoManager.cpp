#include "stdafx.h"
#include "VideoManager.h"


VideoManager::VideoManager()
{
	m_isPlay = false;
	WindowHwnd = CreateWindow(
		WINNAME,				//윈도우 클래스의 이름
		WINNAME,				//윈도우 타이틀바 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일
		WINSTARTX,					    //창의 시작 x좌표
		WINSTARTY,						//창의 시작 y좌표
		WINSIZEX,					//창의 가로크기
		WINSIZEY,					//창의 세로크기
		g_hwnd,				//부모 윈도우
		nullptr,				//메뉴 핸들
		g_hInst,				//인스턴스 지정
		nullptr
	);
}


VideoManager::~VideoManager()
{
	DestroyWindow(WindowHwnd);
}

bool VideoManager::GetPlay()
{
	return m_isPlay;
}

void VideoManager::AddMovie(string Key, _TCHAR * filePath, int width = 0, int height = 0, int XPOS = 0, int YPOS = 0)
{
	VideoInfo info;
	info.filePath = filePath;
	info.height = height;
	info.width = width;
	info.XPOS = XPOS;
	info.YPOS = YPOS;

	VideoMap.insert(make_pair(Key, info));
}

void VideoManager::Play(string strKey)
{
	VideoIter = VideoMap.find(strKey);
	int width = (*VideoIter).second.width;
	int height = (*VideoIter).second.height;
	if (!m_isPlay) 
	{ 
		m_isPlay = true; 
	}
	if (width == 0) { width = WINSIZEX; }
	if (height == 0) { height = WINSIZEY; }

	if (m_MoviehWnd)
	{
		MCIWndClose(m_MoviehWnd);
		MCIWndDestroy(m_MoviehWnd);
		m_MoviehWnd = 0;
	}

	m_MoviehWnd = MCIWndCreate(g_hwnd,
		g_hInst,
		MCIWNDF_NOTIFYMODE | MCIWNDF_NOPLAYBAR | MCIWNDF_NOTIFYPOS | MCIWNDF_SHOWALL,
		(*VideoIter).second.filePath);
	if (m_MoviehWnd)
	{
		SetWindowPos(m_MoviehWnd, g_hwnd, (*VideoIter).second.XPOS, (*VideoIter).second.YPOS, width, height, SWP_NOZORDER);
		MCIWndPlay(m_MoviehWnd);
		m_isPlay = true;
	}
}

void VideoManager::Stop()
{
	if (m_MoviehWnd)
	{
		m_isPlay = false;
		MCIWndStop(m_MoviehWnd);
		MCIWndClose(m_MoviehWnd);
		MCIWndDestroy(m_MoviehWnd);
	}
}

bool VideoManager::MovieCanPlay()
{
	if (m_MoviehWnd)
	{
		return m_isPlay;
	}
	return false;
}

HWND VideoManager::GetVideoHandel()
{
	return m_MoviehWnd;
}

LONG VideoManager::GetEnd()
{
	return MCIWndGetEnd(m_MoviehWnd);
}
