#include "DxD11DemoBase.hpp"
#include "NumericalTypes.hpp"
#include <vector>
#include <dxgi.h>

//---------------------------------------------------------------------------------------
DxD11DemoBase::DxD11DemoBase()
	: driverType(D3D_DRIVER_TYPE_NULL),
	  featureLevel(D3D_FEATURE_LEVEL_11_0),
	  d3dDevice(nullptr),
	  d3dContext(nullptr),
	  swapChain(nullptr),
	  backBufferTarget(nullptr)
{

}

//---------------------------------------------------------------------------------------
DxD11DemoBase::~DxD11DemoBase() 
{
	Shutdown();
}

//---------------------------------------------------------------------------------------
bool DxD11DemoBase::Initialize (
	HINSTANCE hInstance,
	HWND hwnd
) {
	this->hInstance = hInstance;
	this->hwnd = hwnd;

	//-- Get the dimensions of the client window:
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	uint windowWidth = dimensions.right - dimensions.left;
	uint windowHeight = dimensions.bottom - dimensions.top;

	std::vector<D3D_DRIVER_TYPE> driverTypes = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	D3D_FEATURE_LEVEL featureLevels [] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	uint totalFeatureLevels = 3;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	uint creationFlags = 0;
	#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG; 
	#endif

	HRESULT result;
	for (uint driverIndex(0); driverIndex < driverTypes.size(); ++driverIndex) {
		result = D3D11CreateDeviceAndSwapChain (
			0,
			driverTypes[driverIndex],
			0,
			creationFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain,
			&d3dDevice,
			&featureLevel,
			&d3dContext
		);

		if (SUCCEEDED(result)) {
			driverType = driverTypes[driverIndex];
			break;
		}
	}


	if (FAILED(result)) {
		// TODO Dustin - Log an error: "Failed to create the Direct3D device.");
		return false;
	}

	ID3D11Texture2D * backBufferTexture;
	result = swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), 
		reinterpret_cast<LPVOID *>(&backBufferTexture));

	if (FAILED(result)) {
		// TODO Dustin - Log an error 
		// "Failed to get swap chain back buffer!"
		return false;
	}

	result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);

	if (backBufferTexture) backBufferTexture->Release();

	if (FAILED(result)) {
		// TODOD Dustin - Log an error
		// "Failed to create the render target view!"
		return false;
	}

	d3dContext->OMSetRenderTargets(1, &backBufferTarget, 0);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	d3dContext->RSSetViewports(1, &viewport);

	return LoadContent();
}

//---------------------------------------------------------------------------------------
void DxD11DemoBase::Shutdown()
{
	UnloadContent();

	if (backBufferTarget) backBufferTarget->Release();
	if (swapChain) swapChain->Release();
	if (d3dContext) d3dContext->Release();
	if (d3dDevice) d3dDevice->Release();

	d3dDevice = nullptr;
	d3dContext = nullptr;
	swapChain = nullptr;
	backBufferTarget = nullptr;
}

//---------------------------------------------------------------------------------------
bool DxD11DemoBase::LoadContent()
{
	// Override with demo specific content loading.

	return true;
}

//---------------------------------------------------------------------------------------
void DxD11DemoBase::UnloadContent()
{
	// Override with demo specific content unloading.

}


