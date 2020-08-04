#pragma once
class InstanceShader
{
	struct WorldBufferDesc
	{
	public:
		WorldBufferDesc()
		{
			D3DXMatrixIdentity(&World);
		}

		D3DXMATRIX World;
	} worldBufferData;

	ID3D11Buffer* worldBuffer;

	struct ColorBufferDesc
	{
		ColorBufferDesc() : Color(0, 0, 0, 1)
		{

		}

		ColorBufferDesc(D3DXCOLOR color) : Color(color)
		{

		}

		D3DXCOLOR Color;
	} colorBufferData;
	ID3D11Buffer* colorBuffer;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D11ShaderResourceView* m_texture;
	//Ãß°¡
	ID3D11BlendState* blendState[2];
public:
	InstanceShader();
	~InstanceShader();
	bool InitTexture( _tstring filename);
	bool Initialize( _tstring filename);
	void Shutdown();
	bool Render(int, int, D3DXMATRIX, int StartInstanceLocal);
	MAKEGET(ID3D11ShaderResourceView*, m_texture);
private:
	bool InitializeShader(_tstring, _tstring);
	void ReleaseShader();
	void OutputShaderErrorMessage(ID3D10Blob*, _tstring);

	bool SetShaderParameters(D3DXMATRIX &World);
	void RenderShader(int, int, int StartInstanceLocal);
};

