#include "CubeDemo.hpp"

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
CubeDemo::~CubeDemo()
{

}

//---------------------------------------------------------------------------------------
std::shared_ptr<Dx11DemoBase> CubeDemo::getInstance()
{
	static Dx11DemoBase * staticInstance = new CubeDemo();
	if (m_pInstance == nullptr) {
		m_pInstance = std::shared_ptr<Dx11DemoBase>(staticInstance);
	}

	return m_pInstance;
}

//---------------------------------------------------------------------------------------
void CubeDemo::init()
{
	createVertexShaderObject();

	createPixelShaderObject();

	uploadVertexBufferData();
}


//---------------------------------------------------------------------------------------
void CubeDemo::createVertexShaderObject()
{
	ComPtr<ID3DBlob> vsByteCode; // For storing Vertex Shader Bytecode.

	CHECK_DX_ERROR (
		// Read in compiled vertex shader
		D3DReadFileToBlob(L"VertexShader.cso", &vsByteCode);
	)

	CHECK_DX_ERROR (
		m_d3dDevice->CreateVertexShader(vsByteCode->GetBufferPointer(),
			vsByteCode->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
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
			vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), &m_inputLayout);
	)
}

//---------------------------------------------------------------------------------------
void CubeDemo::createPixelShaderObject()
{
	ComPtr<ID3DBlob> psByteCode;

	CHECK_DX_ERROR (
		D3DReadFileToBlob(L"PixelShader.cso", &psByteCode);
	)

	CHECK_DX_ERROR (
		m_d3dDevice->CreatePixelShader(psByteCode->GetBufferPointer(),
			psByteCode->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
	)
}

//---------------------------------------------------------------------------------------
void CubeDemo::uploadVertexBufferData()
{
	vec3 vertices[] = {
		vec3(-0.5f, -0.5f, 0.0f),
		vec3(0.0f, 0.5f, 0.0f),
		vec3(0.5f, -0.5f, 0.0f)
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
void CubeDemo::appLogic(float dt)
{

}

//---------------------------------------------------------------------------------------
void CubeDemo::render()
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
	m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	m_d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3dContext->Draw(3, 0);

	// Swap the back and front buffers.
	m_swapChain->Present(0, 0);
}

//---------------------------------------------------------------------------------------
void CubeDemo::shutdown()
{

}

//---------------------------------------------------------------------------------------
bool CubeDemo::keyInputEvent(
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
bool CubeDemo::mouseButtonEvent (
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
