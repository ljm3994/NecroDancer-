#pragma once

struct InstanceType
{
	InstanceType() : OneMtx(0, 0, 0, 0), TwoMtx(0, 0, 0, 0), ThreeMtx(0, 0, 0, 0), FourMtx(0, 0, 0, 0), Color(0, 0, 0, 0)
	{
		
	}
	D3DXVECTOR4 OneMtx;
	D3DXVECTOR4 TwoMtx;
	D3DXVECTOR4 ThreeMtx;
	D3DXVECTOR4 FourMtx;
	D3DXVECTOR4 Color;
};

struct FrameUvBufferDesc
{
	FrameUvBufferDesc() : FrameUv(0, 0, 1, 1)
	{

	}

	FrameUvBufferDesc(D3DXVECTOR4 Uv) : FrameUv(Uv)
	{

	}

	D3DXVECTOR4 FrameUv;
};

class Model
{
private:
	struct Vertex
	{
		Vertex() {}

		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
	};

	Vertex* vertices;
	D3D11_MAPPED_SUBRESOURCE InstanceData;
	D3D11_MAPPED_SUBRESOURCE UVData;
	ID3D11Buffer* vertexBuffer;
	InstanceType * m_Instance;
	ID3D11Buffer* m_instanceBuffer;
	int m_vertexCount = 6;
	int m_instanceCount = 4;
	int m_StartInstanceLocal = 0;
	FrameUvBufferDesc * FrameUvBufferData;
	ID3D11Buffer* FrameUvBuffer = nullptr;
	UINT maxFrameX = 1;
	UINT maxFrameY = 1;
	UINT CurrentFrameX = 0;
	UINT CurrentFrameY = 0;
	InstanceShader * m_Shader;
	D3DXVECTOR2 m_Pos;
public:
	Model();
	~Model();
	ID3D11ShaderResourceView* GetTexture();
	HRESULT InitBuffer(UINT MaxBufferNum, _tstring FileName);
	void SetMaxInstance();
	void SetOnlyMaxCount(int MaxTile);
	void AddInstanceBuffer(int Index, InstanceType * m_vecPos, FrameUvBufferDesc * UvData);
	void Update();
	void SetStartLocal(int StartInstanceLocal);
	void Render(D3DXMATRIX World);
	void Release();
	MAKEREFGET(UINT, CurrentFrameX);
	MAKEREFGET(UINT, CurrentFrameY);
	MAKEREFGET(UINT, maxFrameX);
	MAKEREFGET(UINT, maxFrameY);
	MAKEGETSET(int, m_vertexCount);
	MAKEGETSET(int, m_instanceCount);
private:
	bool InitializeBuffers(UINT MaxBufferNum);
	void RenderBuffers(D3DXMATRIX World);
	bool LoadTexture( _tstring FileName);


};

