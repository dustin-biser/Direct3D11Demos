#include "IndexRenderingDemo.hpp"

#include "Utils/DebugMessage.hpp"
#include "Utils/CheckDxError.hpp"

#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

#include <iostream>
using namespace std;

#include <glm/glm.hpp>
using glm::vec3;


//---------------------------------------------------------------------------------------
IndexRenderingDemo::~IndexRenderingDemo()
{

}

//---------------------------------------------------------------------------------------
std::shared_ptr<Dx11DemoBase> IndexRenderingDemo::getInstance()
{
	static Dx11DemoBase * staticInstance = new IndexRenderingDemo();
	if (m_pInstance == nullptr) {
		m_pInstance = std::shared_ptr<Dx11DemoBase>(staticInstance);
	}

	return m_pInstance;
}

//---------------------------------------------------------------------------------------
void IndexRenderingDemo::init()
{
	createVertexShaderObject();

	createPixelShaderObject();

	uploadVertexDataToBuffer();
}


//---------------------------------------------------------------------------------------
void IndexRenderingDemo::createVertexShaderObject()
{
	ComPtr<ID3DBlob> vsByteCode; // For storing Vertex Shader Bytecode.

	CHECK_DX_ERROR (
		// Read in compiled vertex shader
		D3DReadFileToBlob(L"VertexShader.cso", &vsByteCode);
	);

	CHECK_DX_ERROR (
		m_d3dDevice->CreateVertexShader(vsByteCode->GetBufferPointer(),
			vsByteCode->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
	);


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
			vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), &m_inputLayout);
	);
}

//---------------------------------------------------------------------------------------
void IndexRenderingDemo::createPixelShaderObject()
{
	ComPtr<ID3DBlob> psByteCode;

	CHECK_DX_ERROR (
		D3DReadFileToBlob(L"PixelShader.cso", &psByteCode);
	);

	CHECK_DX_ERROR (
		m_d3dDevice->CreatePixelShader(psByteCode->GetBufferPointer(),
			psByteCode->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
	);
}

//---------------------------------------------------------------------------------------
void IndexRenderingDemo::uploadVertexDataToBuffer()
{
	//-- Upload vertex positions to m_vertexBuffer:
	{
		vec3 vertices[] = {
			vec3(-0.5f, -0.5f, 0.0f),
			vec3(-0.5f, 0.5f, 0.0f),
			vec3(0.5f, 0.5f, 0.0f),
			vec3(0.5f, -0.5f, 0.0f),
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(vertices);

		D3D11_SUBRESOURCE_DATA initialData;
		ZeroMemory(&initialData, sizeof(initialData));
		initialData.pSysMem = vertices;

		CHECK_DX_ERROR(
			m_d3dDevice->CreateBuffer(&vertexBufferDesc, &initialData,
				m_vertexBuffer.GetAddressOf());
		);
	}


	//-- Upload triangle indices to m_indexBuffer:
	{
		ushort indices[] = {
			0,1,2,
			3,0,2
		};

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = sizeof(indices);

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = indices;

		CHECK_DX_ERROR (
			m_d3dDevice->CreateBuffer(&indexBufferDesc, &resourceData,
				m_indexBuffer.GetAddressOf());
		);

	}

}

//---------------------------------------------------------------------------------------
void IndexRenderingDemo::appLogic(float dt)
{

}

//---------------------------------------------------------------------------------------
void IndexRenderingDemo::render()
{
	if (!m_d3dContext) return;

	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	// Clear the back buffer with the clearColor.
	m_d3dContext->ClearRenderTargetView(m_backBufferTarget.Get(), clearColor);

	uint stride = sizeof(vec3);
	uint offset = 0;

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride,
		&offset);
	m_d3dContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	m_d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dContext->DrawIndexed(6, 0, 0);

	// Swap the back and front buffers.
	m_swapChain->Present(0, 0);
}

//---------------------------------------------------------------------------------------
void IndexRenderingDemo::shutdown()
{

}

//---------------------------------------------------------------------------------------
bool IndexRenderingDemo::keyInputEvent(
	HWND hWindow,
	UINT message,
	WPARAM vKey
) {
	bool eventHandled(false);

	if (message == WM_KEYDOWN) {
		if (vKey == 'A') {
			cout << "A Key Pressed" << endl;
			eventHandled = true;
		}
	}
	if (message == WM_KEYUP) {
		if (vKey == 'A') {
			cout << "A Key Released" << endl;
			eventHandled = true;
		}
	}

	return eventHandled;
}

//---------------------------------------------------------------------------------------
bool IndexRenderingDemo::mouseButtonEvent (
	HWND hWindow,
	UINT message,
	WPARAM wParam
) {
	bool eventHandled(false);
	if (message == WM_LBUTTONDOWN) {
		cout << "Left MouseButton Down" << endl;
		eventHandled = true;
	}

	return eventHandled;
}
