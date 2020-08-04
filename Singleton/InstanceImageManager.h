#pragma once
#include "SingletonBase.h"

class InstanceImageManager : public SingletonBase<InstanceImageManager>
{
private:
	typedef map<string, InstanceImage*>				ImgMap;//맵으로 만든 이미지 목록
	typedef map<string, InstanceImage*>::iterator	ImgIter;//맵으로 만든 이미지 목록의 반복자

	ImgMap m_ImgMap;
	Rect* rect;
	Circle* circle;
public:
	InstanceImage* AddImage(string strKey, _tstring file, UINT MaxFrameX, UINT MaxFrameY, UINT MaxBuffer);

	//이미지 키값으로 찾기
	InstanceImage* GetImage(string strkey);
	//이미지 키값으로 삭제
	bool DeleteImage(string strKey);
	//전부 삭제
	void DeleteAll();
	//일반렌더
	void Render(string strKey);

	//렉트랑 서클도 이미지 매니저가 하나씩 들고있다.
	void RectRender() { rect->Render(); }
	void CircleRender() { circle->Render(); }

	MAKEGET(Rect*, rect);
	MAKEGET(Circle*, circle);

	InstanceImageManager() { rect = new Rect(); circle = new Circle(72); };
	~InstanceImageManager() { SAFE_DELETE(rect) SAFE_DELETE(circle) };
};

