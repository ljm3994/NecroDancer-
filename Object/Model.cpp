#include "stdafx.h"
#include "Model.h"


Model::Model()
{

}


Model::~Model()
{

}

ID3D11ShaderResourceView * Model::GetTexture()
{
	return m_Shader->Getm_texture();
}

HRESULT Model::InitBuffer(UINT MaxBufferNum, _tstring FileName)
{
	bool result;
	
	result = InitializeBuffers(MaxBufferNum);

	if (!result)
	{
		return S_FALSE;
	}

	result = LoadTexture(FileName);

	if (!result)
	{
		return S_FALSE;
	}

	return S_OK;
}
// 인스턴스 버퍼에 데이터를 세팅한다.
void Model::SetMaxInstance()
{
	DeviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &InstanceData);
	DeviceContext->Map(FrameUvBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &UVData);

	m_Instance = (InstanceType*)InstanceData.pData;
	FrameUvBufferData = (FrameUvBufferDesc*)UVData.pData;
}

void Model::SetOnlyMaxCount(int MaxTile)
{
	m_instanceCount = MaxTile;

	m_Instance = new InstanceType[m_instanceCount];

	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	HRESULT hr;
	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = m_Instance;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;
	if (m_instanceBuffer)
	{
		SAFE_RELEASE(m_instanceBuffer);
	}
	hr = Device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);

	if (FAILED(hr))
	{
		return;
	}

	FrameUvBufferData = new FrameUvBufferDesc[m_instanceCount];

	D3D11_BUFFER_DESC desc = { 0 };
	D3D11_SUBRESOURCE_DATA uvData;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(FrameUvBufferDesc) * m_instanceCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	uvData.pSysMem = FrameUvBufferData;
	uvData.SysMemPitch = 0;
	uvData.SysMemSlicePitch = 0;
	if (FrameUvBuffer)
	{
		SAFE_RELEASE(FrameUvBuffer);
	}
	hr = Device->CreateBuffer(&desc, &uvData, &FrameUvBuffer);
	assert(SUCCEEDED(hr));
}

void Model::AddInstanceBuffer(int Index, InstanceType * m_vecPos, FrameUvBufferDesc * UvData)
{
	m_Instance[Index] = *m_vecPos;
	FrameUvBufferData[Index] = *UvData;
}

void Model::Update()
{
	if (m_instanceCount > 0)
	{
		DeviceContext->Unmap(m_instanceBuffer, 0);
		DeviceContext->Unmap(FrameUvBuffer, 0);
	}
}

void Model::SetStartLocal(int StartInstanceLocal)
{
	m_StartInstanceLocal = StartInstanceLocal;
}

void Model::Render(D3DXMATRIX World)
{
	RenderBuffers(World);
}

bool Model::InitializeBuffers(UINT MaxBufferNum)
{
	vertices = new Vertex[m_vertexCount];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	
	//uv좌표는 왼쪽상단이 0,0 최대값은 1이다.
	vertices[0].Uv = D3DXVECTOR2(0.0f, 1.0f);
	vertices[1].Uv = D3DXVECTOR2(0.0f, 0.0f);
	vertices[2].Uv = D3DXVECTOR2(1.0f, 1.0f);
	vertices[3].Uv = D3DXVECTOR2(1.0f, 1.0f);
	vertices[4].Uv = D3DXVECTOR2(0.0f, 0.0f);
	vertices[5].Uv = D3DXVECTOR2(1.0f, 0.0f);

	HRESULT hr; 
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		hr = Device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

		if (FAILED(hr))
		{
			return false;
		}
	}
	m_instanceCount = MaxBufferNum;
	m_Instance = new InstanceType[MaxBufferNum];

	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * MaxBufferNum;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = m_Instance;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;
	if (m_instanceBuffer)
	{
		SAFE_RELEASE(m_instanceBuffer);
	}
	hr = Device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);

	if (FAILED(hr))
	{
		return false;
	}

	FrameUvBufferData = new FrameUvBufferDesc[MaxBufferNum];

	D3D11_BUFFER_DESC desc = { 0 };
	D3D11_SUBRESOURCE_DATA uvData;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(FrameUvBufferDesc) * MaxBufferNum;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	uvData.pSysMem = FrameUvBufferData;
	uvData.SysMemPitch = 0;
	uvData.SysMemSlicePitch = 0;
	if (FrameUvBuffer)
	{
		SAFE_RELEASE(FrameUvBuffer);
	}
	hr = Device->CreateBuffer(&desc, &uvData, &FrameUvBuffer);
	assert(SUCCEEDED(hr));

	return true;
}

void Model::RenderBuffers(D3DXMATRIX World)
{
	UINT strides[3];
	UINT offsets[3];
	ID3D11Buffer* bufferPointers[3];

	strides[0] = sizeof(Vertex);
	strides[1] = sizeof(InstanceType);
	strides[2] = sizeof(FrameUvBufferDesc);

	offsets[0] = 0;
	offsets[1] = m_StartInstanceLocal;
	offsets[2] = 0;

	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;
	bufferPointers[2] = FrameUvBuffer;

	DeviceContext->IASetVertexBuffers(0, 3, bufferPointers, strides, offsets);

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Shader->Render(m_vertexCount, m_instanceCount, World, m_StartInstanceLocal);
}

bool Model::LoadTexture(_tstring FileName)
{
	bool result;

	// Create the texture object.
	m_Shader = new InstanceShader();
	if (!m_Shader)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Shader->Initialize(FileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Release()
{
	SAFE_DELETE_ARRAY(vertices);
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}

	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
	}

	if (FrameUvBuffer)
	{
		FrameUvBuffer->Release();
	}

	if (m_Instance)
	{
		SAFE_DELETE_ARRAY(m_Instance);
	}

	if (FrameUvBufferData)
	{
		SAFE_DELETE_ARRAY(FrameUvBufferData);
	}
	if (m_Shader)
	{
		m_Shader->Shutdown();
		SAFE_DELETE(m_Shader);
	}
}
