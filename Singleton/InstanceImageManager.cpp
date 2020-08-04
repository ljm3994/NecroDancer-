#include "stdafx.h"
#include "InstanceImageManager.h"

InstanceImage * InstanceImageManager::AddImage(string strKey, _tstring file, UINT MaxFrameX, UINT MaxFrameY, UINT MaxBuffer)
{
	InstanceImage* img = GetImage(strKey);

	//추가하려는 이미지가 이미 존재한다면 새로만들지 않고 리턴
	if (img) return img;

	//존재하지 않는다면
	img = new InstanceImage();
	img->Init(file, MaxBuffer, MaxFrameX, MaxFrameY);
	//생성된 이미지를 맵으로 만든 목록에 추가한다
	m_ImgMap.insert(make_pair(strKey, img));
	return img;
}

InstanceImage * InstanceImageManager::GetImage(string strkey)
{
	//해당 키값으로 탐색 하는데 키값이 없으면 맨마지막인 end 가 반환된다.
	ImgIter iter = m_ImgMap.find(strkey);

	//검색한 키를 찾았다면
	if (iter != m_ImgMap.end())
	{
		return iter->second;
	}
	//못찾으면 nullptr반환
	return nullptr;
}

bool InstanceImageManager::DeleteImage(string strKey)
{
	//해당키를 검색
	ImgIter iter = m_ImgMap.find(strKey);

	//검색한 키를 찾았다면
	if (iter != m_ImgMap.end())
	{
		//이미지 만든것 릴리즈후 동적할당 해제
		SAFE_DELETE(iter->second);
		//맵에서 중간삭제
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

