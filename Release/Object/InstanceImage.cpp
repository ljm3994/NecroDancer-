#include "stdafx.h"
#include "InstanceImage.h"


InstanceImage::InstanceImage()
{
}


InstanceImage::~InstanceImage()
{
	
}

void InstanceImage::Init(_tstring FileName, UINT MaxInstanceBufferNum, UINT MaxFramX, UINT MaxFramY)
{
	m_Model = new Model();
	m_Model->InitBuffer(MaxInstanceBufferNum, FileName);
	m_MaxFramX = MaxFramX;
	m_MaxFramY = MaxFramY;
}

int InstanceImage::AddTexture()
{
	TileTexture * Temp = new TileTexture();
	m_Texture.emplace_back(Temp);
	return m_Texture.size() - 1;
}

void InstanceImage::Position(int Index, float x, float y)
{
	Position(Index, D3DXVECTOR2(x, y));
}

void InstanceImage::Color(int Index, float r, float g, float b, float a)
{
	Color(Index, D3DXVECTOR4(r, g, b, a));
}

void InstanceImage::Color(int Index, D3DXVECTOR4 col)
{
	m_Texture[Index]->Color(col);
}

void InstanceImage::SetInstanceLocal(int InstanceLocal)
{
	m_Model->SetStartLocal(InstanceLocal);
}

void InstanceImage::Position(int Index, D3DXVECTOR2 vec)
{
	m_Texture[Index]->Position(vec);
}

void InstanceImage::Scale(int Index, float x, float y)
{
	Scale(Index, D3DXVECTOR2(x, y));
}

void InstanceImage::TextureUV(int Index, float Uvx, float Uvy)
{
	D3DXVECTOR4 TempVec;

	TempVec.x = 1.0f / m_MaxFramX * Uvx;
	TempVec.y = 1.0f / m_MaxFramY * Uvy;
	TempVec.z = 1.0f / m_MaxFramX * (Uvx + 1);
	TempVec.w = 1.0f / m_MaxFramY * (Uvy + 1);


	m_Texture[Index]->UVData(TempVec.x, TempVec.y, TempVec.z, TempVec.w, static_cast<int>(Uvx), static_cast<int>(Uvy));
}

int InstanceImage::GetTexturUvX(int Index)
{
	return m_Texture[Index]->Getm_FrameX();
}

int InstanceImage::GetTexturUvY(int Index)
{
	return m_Texture[Index]->Getm_FrameY();
}

void InstanceImage::Scale(int Index, D3DXVECTOR2 vec)
{
	m_Texture[Index]->Scale(vec);
}

void InstanceImage::Rotate(int Index, D3DXVECTOR3 x)
{
	m_Texture[Index]->Rotate(x);
}
void InstanceImage::Rotate(int Index, float x, float y, float z)
{
	Rotate(Index, D3DXVECTOR3(x, y, z));
}

D3DXVECTOR4 InstanceImage::GetUvData(int Index)
{
	return m_Texture[Index]->Getm_UvData()->FrameUv;
}

void InstanceImage::SetTextureUpdate(int Index)
{
	m_Texture[Index]->Update();
	AddInstance(Index);
}

void InstanceImage::MaxModel(int Max)
{
	m_Model->SetMaxInstance();
}

void InstanceImage::SetOnlyMax(int Max)
{
	m_Model->SetOnlyMaxCount(Max);
}

void InstanceImage::AddInstance(int Index)
{
	m_Model->AddInstanceBuffer(Index, m_Texture[Index]->Getm_Matrix(), m_Texture[Index]->Getm_UvData());
}

void InstanceImage::SetSight(int Index, bool Sight)
{
	m_Texture[Index]->Setm_Sight(Sight);
}

bool InstanceImage::GetSight(int Index)
{
	return m_Texture[Index]->Getm_Sight();
}

void InstanceImage::SetLight(int Index, bool Light)
{
	m_Texture[Index]->Setm_Light(Light);
}

bool InstanceImage::GetLight(int Index)
{
	return m_Texture[Index]->Getm_Light();
}

void InstanceImage::SetPatern(int Index, bool Patern)
{
	m_Texture[Index]->SetPatern(Patern);
}

bool InstanceImage::GetPatern(int Index)
{
	return m_Texture[Index]->GetPatern();
}

bool InstanceImage::GetFirstLight(int Index)
{
	return m_Texture[Index]->Getm_FirstLight();
}

void InstanceImage::SetFirstLight(int Index, bool Light)
{
	m_Texture[Index]->Setm_FirstLight(Light);
}

void InstanceImage::Clear()
{
	for (UINT i = 0; i < m_Texture.size(); i++)
	{
		if (m_Texture[i])
		{
			SAFE_DELETE(m_Texture[i]);
		}
	}
	m_Texture.shrink_to_fit();
	m_Texture.clear();
}

void InstanceImage::Update()
{
	m_Model->Update();
}

void InstanceImage::Render()
{
	m_Model->Render(m_WorldMatrix);
}

void InstanceImage::Release()
{
	m_Model->Release();
	SAFE_DELETE(m_Model);
	Clear();
}
