#pragma once
#include "Object/TileTexture.h"

 class InstanceImage
{
	 D3DXMATRIX m_WorldMatrix;
	 vector<TileTexture*> m_Texture;
	 UINT m_MaxFramX;
	 UINT m_MaxFramY;
	 D3DXVECTOR2 position;
	 D3DXVECTOR2 scale;
	 D3DXVECTOR3 angle;
	 D3DXVECTOR3 angle2;
	 Model * m_Model;
public:
	InstanceImage();
	~InstanceImage();
	void Init(_tstring FileName, UINT MaxInstanceBufferNum, UINT MaxFramX, UINT MaxFramY);
	int AddTexture();
	void Position(int Index, float x, float y);
	void Color(int Index, float r, float g, float b, float a);
	void Color(int Index, D3DXVECTOR4 col);
	void SetInstanceLocal(int InstanceLocal);
	void Position(int Index, D3DXVECTOR2 vec);
	void Scale(int Index, float x, float y);
	void TextureUV(int Index, float Uvx, float Uvy);
	int GetTexturUvX(int Index);
	int GetTexturUvY(int Index);
	void Scale(int Index, D3DXVECTOR2 vec);
	void Rotate(int Index, D3DXVECTOR3 x);
	void Rotate(int Index, float x, float y, float z);
	D3DXVECTOR4 GetUvData(int Index);
	void SetTextureUpdate(int Index);
	void MaxModel(int Max);
	void SetOnlyMax(int Max);
	void AddInstance(int Index);
	void SetSight(int Index, bool Sight);
	bool GetSight(int Index);
	void SetLight(int Index, bool Light);
	bool GetLight(int Index);
	void SetPatern(int Index, bool Patern);
	bool GetPatern(int Index);
	bool GetFirstLight(int Index);
	void SetFirstLight(int Index, bool Light);
	void Clear();
	void Update();
	void Render();
	void Release();
};

