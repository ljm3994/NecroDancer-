#pragma once
#include "SingletonBase.h"

struct VideoInfo
{
	_TCHAR * filePath;
	int width;
	int height;
	int XPOS;
	int YPOS;
};
class VideoManager : public SingletonBase<VideoManager>
{
private:
	map<string, VideoInfo>					VideoMap;
	map<string, VideoInfo>::iterator	VideoIter;
	HWND m_MoviehWnd;
	bool m_isPlay;
	HWND WindowHwnd;
public:
	VideoManager();
	~VideoManager();
	
	bool GetPlay();
	void AddMovie(string Key, _TCHAR * filePath, int width, int height, int XPOS, int YPOS);
	void Play(string strKey);
	void Stop();
	bool MovieCanPlay();
	HWND GetVideoHandel();
	LONG GetEnd();
	MAKEGETSET(bool, m_isPlay);
};

