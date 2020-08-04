#pragma once
class TileTexture
{
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR4 m_Color;
	int m_FrameX;
	int m_FrameY;
	InstanceType * m_Matrix;
	FrameUvBufferDesc * m_UvData;
	bool m_Sight;
	bool m_Light;
	bool m_FirstLight;
	bool Patern;
public:
	TileTexture();
	~TileTexture();
	void Position(float x, float y);
	void Position(D3DXVECTOR2 vec);

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2 vec);
	void Color(float r, float g, float b, float a);
	void Color(D3DXVECTOR4 col);
	void Rotate(D3DXVECTOR3 x);
	void Rotate(float x, float y, float z);
	void UVData(float Xpos, float Ypos, float Wpos, float Zpos, int originX, int originY);
	void Update();
	MAKEGET(InstanceType*, m_Matrix);
	MAKEGET(FrameUvBufferDesc*, m_UvData);
	MAKEGET(int, m_FrameX);
	MAKEGET(int, m_FrameY);
	MAKEGETSET(bool, m_Sight);
	MAKEGETSET(bool, m_Light);
	MAKEGETSET(bool, m_FirstLight);
	MAKEGETSET(bool, Patern);
};

