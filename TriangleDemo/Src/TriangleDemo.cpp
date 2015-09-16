#include "TriangleDemo.hpp"

#include "Utils/DebugMessage.hpp"
#include "Utils/CheckDxError.hpp"

#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;


//---------------------------------------------------------------------------------------
TriangleDemo::~TriangleDemo()
{

}

//---------------------------------------------------------------------------------------
std::shared_ptr<Dx11DemoBase> TriangleDemo::getInstance()
{
	static Dx11DemoBase * staticInstance = new TriangleDemo();
	if (m_pInstance == nullptr) {
		m_pInstance = std::shared_ptr<Dx11DemoBase>(staticInstance);
	}

	return m_pInstance;
}

//---------------------------------------------------------------------------------------
void TriangleDemo::init()
{
	createVertexShaderObject();

	createPixelShaderObject();

	uploadVertexBufferData();
}


//---------------------------------------------------------------------------------------
void TriangleDemo::createVertexShaderObject()
{
	ComPtr<ID3DBlob> vsBlob; // Vertex Shader Blob.

	CHECK_DX_ERROR (
		D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);
	)

	CHECK_DX_ERROR (
		m_d3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
	)

	D3D11_INPUT_ELEMENT_DESC inputVertexDataLayout;
	inputVertexDataLayout.SemanticName = "POSITION";
	inputVertexDataLayout.SemanticIndex = 0;
	inputVertexDataLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputVertexDataLayout.InputSlot = 0;
	inputVertexDataLayout.AlignedByteOffset = 0;
	inputVertexDataLayout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputVertexDataLayout.InstanceDataStepRate = 0;

	CHECK_DX_ERROR (
		m_d3dDevice->CreateInputLayout(&inputVertexDataLayout, 1,
			vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
	)
}

//---------------------------------------------------------------------------------------
void TriangleDemo::createPixelShaderObject()
{
	ComPtr<ID3DBlob> psBlob;

	CHECK_DX_ERROR (
		D3DReadFileToBlob(L"PixelShader.cso", &psBlob);
	)

	CHECK_DX_ERROR (
		m_d3dDevice->CreatePixelShader(psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
	)
}

//---------------------------------------------------------------------------------------
void TriangleDemo::uploadVertexBufferData()
{
	XMFLOAT3 vertices[] = {
		XMFLOAT3(-0.5f, -0.5f, 0.0f),
		XMFLOAT3(0.0f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f)
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(vertices);

	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = vertices;

	CHECK_DX_ERROR (
		m_d3dDevice->CreateBuffer(&vertexBufferDesc, &initialData,
			m_vertexBuffer.GetAddressOf());
	)
}

//---------------------------------------------------------------------------------------
void TriangleDemo::appLogic(float dt)
{

}

//---------------------------------------------------------------------------------------
void TriangleDemo::render()
{
	if (!m_d3dContext) return;

	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	// Clear the back buffer with the clearColor.
	m_d3dContext->ClearRenderTargetView(m_backBufferTarget.Get(), clearColor);

	uint stride = sizeof(XMFLOAT3);
	uint offset = 0;

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride,
		&offset);
	m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	m_d3dContext->Draw(3, 0);

	// Swap the back and front buffers.
	m_swapChain->Present(0, 0);
}

//---------------------------------------------------------------------------------------
void TriangleDemo::shutdown()
{

}
