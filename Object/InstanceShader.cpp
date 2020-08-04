#include "stdafx.h"
#include "InstanceShader.h"


InstanceShader::InstanceShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}


InstanceShader::~InstanceShader()
{
}

bool InstanceShader::InitTexture(_tstring filename)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(Device, filename.c_str(), NULL, NULL, &m_texture, NULL);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool InstanceShader::Initialize(_tstring filename)
{
	bool result;

	result = InitializeShader(_T("./Shader/InstancevsShader.hlsl"), _T("./Shader/InstancepsShader.hlsl"));
	if (!result)
	{
		return false;
	}

	_tstring textureFile = _T("./images/") + filename;
	result = InitTexture(textureFile);
	if (!result)
	{
		return false;
	}

	return false;
}

void InstanceShader::Shutdown()
{
	ReleaseShader();
}

bool InstanceShader::Render(int vertexCount, int instanceCount, D3DXMATRIX WorldMatrix, int StartInstanceLocal)
{
	bool result;

	result = SetShaderParameters(WorldMatrix);

	if (!result)
	{
		return false;
	}
	RenderShader(vertexCount, instanceCount, StartInstanceLocal);
	return true;
}

bool InstanceShader::InitializeShader(_tstring vsFilename, _tstring psFileName)
{

	HRESULT result;
	UINT numElements;

	result = D3DX11CompileFromFile(vsFilename.c_str(), 0, 0, "InstanceVertexShader", "vs_5_0", NULL, 0, 0, &vertexShaderBuffer, 0, 0);

	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CompileFromFile(psFileName.c_str(), 0, 0, "InstancePixelShader", "ps_5_0", NULL, 0, 0, &pixelShaderBuffer, 0, 0);


	if (FAILED(result))
	{
		return false;
	}

	result = Device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);

	if (FAILED(result))
	{
		return false;
	}

	result = Device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);

	if (FAILED(result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "TEXCOORD", 5, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{ "TEXCOORD", 6, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};

	numElements = sizeof(LayoutDesc) / sizeof(LayoutDesc[0]);

	result = Device->CreateInputLayout(LayoutDesc, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(WorldBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &worldBuffer);
		assert(SUCCEEDED(hr));
	}

	

	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Device->CreateBlendState(&desc, &blendState[0]);

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Device->CreateBlendState(&desc, &blendState[1]);
	}

	//Create WorldBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(WorldBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &worldBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create ColorBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ColorBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &colorBuffer);
		assert(SUCCEEDED(hr));
	}

	return true;
}

void InstanceShader::ReleaseShader()
{
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void InstanceShader::OutputShaderErrorMessage(ID3D10Blob *, _tstring )
{

}

bool InstanceShader::SetShaderParameters(D3DXMATRIX &World)
{
	D3DXMatrixTranspose(&worldBufferData.World, &World);

	return true;
}
// ½¦ÀÌ´õ ·£´õ
void InstanceShader::RenderShader(int vertexCount, int instanceCount, int StartInstanceLocal)
{
	DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, &colorBufferData, 0, 0);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);

	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, &worldBufferData, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);

	DeviceContext->IASetInputLayout(m_layout);
	
	DeviceContext->PSSetShaderResources(0,//½ºÅ¸Æ® ½½·Ô
		1,//¸®¼Ò½º °¹¼ö
		&m_texture);//¼ÎÀÌ´õÀÚ¿øºä

	DeviceContext->VSSetShader(m_vertexShader, NULL, 0);
	DeviceContext->PSSetShader(m_pixelShader, NULL, 0);
	
	DeviceContext->OMSetBlendState(blendState[1], NULL, 0xff);

	DeviceContext->DrawInstanced(vertexCount, instanceCount, 0, StartInstanceLocal);
	//ºí·»µå ²ô±â
	DeviceContext->OMSetBlendState(blendState[0], NULL, 0xff);
}
