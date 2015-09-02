#ifdef _WIN32
	// Allow use of freopen() without compilation warnings/errors.
	#define _CRT_SECURE_NO_DEPRECATE
	#include <cstdio>
#endif

#include "Dx11DemoBase.hpp"
#include "NumericalTypes.hpp"
#include "Exception.hpp"

#include <vector>
#include <iostream>
using std::cerr;
using std::endl;

#include <dxgi.h>

//---------------------------------------------------------------------------------------
Dx11DemoBase::Dx11DemoBase (
	uint width, 
	uint height,
	std::wstring name
) : 
	  width(width),
	  height(height),
	  driverType(D3D_DRIVER_TYPE_NULL),
	  featureLevel(D3D_FEATURE_LEVEL_11_0),
	  d3dDevice(nullptr),
	  d3dContext(nullptr),
	  swapChain(nullptr),
	  backBufferTarget(nullptr)
{

}

//---------------------------------------------------------------------------------------
Dx11DemoBase::~Dx11DemoBase() 
{
	Shutdown();
}

//---------------------------------------------------------------------------------------
int Dx11DemoBase::Run (
	HINSTANCE hInstance,
	int nCmdShow
) {
	// Open a new console window
	AllocConsole();

	//-- Associate std input/output with newly opened console window:
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "DX11WindowClass";

	// Register a window class with name "DX11WindowClass"
	if (!RegisterClassEx(&wndClass))
		return -1;

	RECT windowRect = {0, 0, width, height};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Now create the window from the previously registered "DX11WindowClass" class 
	hwnd = CreateWindowA(
		"DX11WindowClass",
		"Blank Direct3D Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	if (!hwnd)
		return -1;

	ShowWindow(hwnd, nCmdShow);
	
	MSG msg = { 0 };

	try {
		Initialize();

		MainApplicationLoop(msg);
	}
	catch (const std::exception & e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cerr << "Unrecognized exception caught" << endl;
	}

	// Release resources
	Shutdown();

	return static_cast<int>(msg.wParam);
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::MainApplicationLoop(MSG & msg) {
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			// Translates input virtual key messages to ascii char key messages.
			TranslateMessage(&msg);

			// Disptach message to our registered Window Procedure Callback function.
			DispatchMessage(&msg);
		}
		else {
			float dt = 0.0f;
			//-- Call overriden child-class methods:
			Update(dt);
			Render();
		}
	}
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::Initialize () {
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
		throw Exception("Failed to create the Direct3D device.");
	}

	ID3D11Texture2D * backBufferTexture;
	result = swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), 
			reinterpret_cast<LPVOID *>(&backBufferTexture) );
	if (FAILED(result)) {
		throw Exception("Failed to get swap chain back buffer!");
	}

	result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);

	if (backBufferTexture) backBufferTexture->Release();

	if (FAILED(result)) {
		throw Exception("Failed to create the Render Target View!");
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

	LoadContent();
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::Shutdown()
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
bool Dx11DemoBase::LoadContent()
{
	// Override with demo specific content loading.

	return true;
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::UnloadContent()
{
	// Override with demo specific content unloading.

}

//---------------------------------------------------------------------------------------
LRESULT CALLBACK Dx11DemoBase::WindowProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
) {
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message) {
	case WM_PAINT:
		//-- Draw window background:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;

	case WM_DESTROY: 
		// Send a WM_QUIT message to terminate our application loop within wWinMain().
		PostQuitMessage(0);
		break;

	default:
		// Pass unprocessed messages to the default windows procedure callback.
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
