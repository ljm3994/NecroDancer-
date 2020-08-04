#include "stdafx.h"
#include "TileTexture.h"


TileTexture::TileTexture()
{
	m_Matrix = new InstanceType();
	m_UvData = new FrameUvBufferDesc();
	m_Color = D3DXVECTOR4(0, 0, 0, 0);
	m_Sight = false;
	m_Light = false;
	m_FirstLight = false;
	Patern = false;
}


TileTexture::~TileTexture()
{
	SAFE_DELETE(m_Matrix);
	SAFE_DELETE(m_UvData);
}

void TileTexture::Position(float Xpos, float Ypos)
{
	Position(D3DXVECTOR2(Xpos, Ypos));
}

void TileTexture::Position(D3DXVECTOR2 vec)
{
	m_Pos = D3DXVECTOR3(vec.x, vec.y, 0.0f);
}

void TileTexture::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void TileTexture::Scale(D3DXVECTOR2 vec)
{
	m_Scale = D3DXVECTOR3(vec.x, vec.y, 0.0f);
}

void TileTexture::Color(float r, float g, float b, float a)
{
	Color(D3DXVECTOR4(r, g, b, a));
}

void TileTexture::Color(D3DXVECTOR4 col)
{
	m_Color = col;
}

void TileTexture::Rotate(D3DXVECTOR3 x)
{
	m_Rotation = x;
}

void TileTexture::Rotate(float x, float y, float z)
{
	Rotate(D3DXVECTOR3(x, y, z));
}

void TileTexture::UVData(float Xpos, float Ypos, float Wpos, float Zpos, int originX, int originY)
{
	m_FrameX = originX;
	m_FrameY = originY;
	m_UvData->FrameUv = D3DXVECTOR4(Xpos, Ypos, Wpos, Zpos);
}


void TileTexture::Update()
{
	D3DXMATRIX S, R, T, W;
	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, 1);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&T, m_Pos.x, m_Pos.y, 0);
	W = S * R * T;

	m_Matrix->OneMtx.x = W._11;
	m_Matrix->OneMtx.y = W._12;
	m_Matrix->OneMtx.z = W._13;
	m_Matrix->OneMtx.w = W._14;
	m_Matrix->TwoMtx.x = W._21;
	m_Matrix->TwoMtx.y = W._22;
	m_Matrix->TwoMtx.z = W._23;
	m_Matrix->TwoMtx.w = W._24;
	m_Matrix->ThreeMtx.x = W._31;
	m_Matrix->ThreeMtx.y = W._32;
	m_Matrix->ThreeMtx.z = W._33;
	m_Matrix->ThreeMtx.w = W._34;
	m_Matrix->FourMtx.x = W._41;
	m_Matrix->FourMtx.y = W._42;
	m_Matrix->FourMtx.z = W._43;
	m_Matrix->FourMtx.w = W._44;
	m_Matrix->Color = m_Color;
}
