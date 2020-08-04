#include "stdafx.h"
#include "InstanceImageManager.h"

InstanceImage * InstanceImageManager::AddImage(string strKey, _tstring file, UINT MaxFrameX, UINT MaxFrameY, UINT MaxBuffer)
{
	InstanceImage* img = GetImage(strKey);

	//�߰��Ϸ��� �̹����� �̹� �����Ѵٸ� ���θ����� �ʰ� ����
	if (img) return img;

	//�������� �ʴ´ٸ�
	img = new InstanceImage();
	img->Init(file, MaxBuffer, MaxFrameX, MaxFrameY);
	//������ �̹����� ������ ���� ��Ͽ� �߰��Ѵ�
	m_ImgMap.insert(make_pair(strKey, img));
	return img;
}

InstanceImage * InstanceImageManager::GetImage(string strkey)
{
	//�ش� Ű������ Ž�� �ϴµ� Ű���� ������ �Ǹ������� end �� ��ȯ�ȴ�.
	ImgIter iter = m_ImgMap.find(strkey);

	//�˻��� Ű�� ã�Ҵٸ�
	if (iter != m_ImgMap.end())
	{
		return iter->second;
	}
	//��ã���� nullptr��ȯ
	return nullptr;
}

bool InstanceImageManager::DeleteImage(string strKey)
{
	//�ش�Ű�� �˻�
	ImgIter iter = m_ImgMap.find(strKey);

	//�˻��� Ű�� ã�Ҵٸ�
	if (iter != m_ImgMap.end())
	{
		//�̹��� ����� �������� �����Ҵ� ����
		SAFE_DELETE(iter->second);
		//�ʿ��� �߰�����
		m_ImgMap.erase(iter);
		return true;
	}
	return false;
}

void InstanceImageManager::DeleteAll()
{
	ImgIter iter = m_ImgMap.begin();
	for (iter; iter != m_ImgMap.end();)
	{
		if (iter->second != nullptr)
		{
			SAFE_DELETE(iter->second);
			iter = m_ImgMap.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_ImgMap.clear();
}

void InstanceImageManager::Render(string strKey)
{
	InstanceImage* img = GetImage(strKey);
	if (img) img->Render();
}

