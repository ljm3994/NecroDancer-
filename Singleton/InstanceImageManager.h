#pragma once
#include "SingletonBase.h"

class InstanceImageManager : public SingletonBase<InstanceImageManager>
{
private:
	typedef map<string, InstanceImage*>				ImgMap;//������ ���� �̹��� ���
	typedef map<string, InstanceImage*>::iterator	ImgIter;//������ ���� �̹��� ����� �ݺ���

	ImgMap m_ImgMap;
	Rect* rect;
	Circle* circle;
public:
	InstanceImage* AddImage(string strKey, _tstring file, UINT MaxFrameX, UINT MaxFrameY, UINT MaxBuffer);

	//�̹��� Ű������ ã��
	InstanceImage* GetImage(string strkey);
	//�̹��� Ű������ ����
	bool DeleteImage(string strKey);
	//���� ����
	void DeleteAll();
	//�Ϲݷ���
	void Render(string strKey);

	//��Ʈ�� ��Ŭ�� �̹��� �Ŵ����� �ϳ��� ����ִ�.
	void RectRender() { rect->Render(); }
	void CircleRender() { circle->Render(); }

	MAKEGET(Rect*, rect);
	MAKEGET(Circle*, circle);

	InstanceImageManager() { rect = new Rect(); circle = new Circle(72); };
	~InstanceImageManager() { SAFE_DELETE(rect) SAFE_DELETE(circle) };
};

