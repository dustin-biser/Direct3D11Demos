#ifdef _WIN32
	// Allow use of freopen() without compilation warnings/errors.
	#define _CRT_SECURE_NO_DEPRECATE
	#include <cstdio>
#endif

#include "Dx11DemoBase.hpp"
#include "NumericalTypes.hpp"
#include "Exception.hpp"
#include "Utils/CheckDxError.hpp"
#include "Utils/DebugMessage.hpp"

#include <vector>
#include <iostream>
#include <cassert>
using std::cerr;
using std::endl;

#include <dxgi.h>


//---------------------------------------------------------------------------------------
Dx11DemoBase::Dx11DemoBase (
	uint width, 
	uint height,
	std::wstring name
) : 
	  m_width(width),
	  m_height(height),
	  m_driverType(D3D_DRIVER_TYPE_NULL),
	  m_featureLevel(D3D_FEATURE_LEVEL_11_0)
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
	//-- Use the following code to open a new console window and redirect stdout to it:
	{
		//// Open a new console window
		//AllocConsole();

		////-- Associate std input/output with newly opened console window:
		//freopen("CONIN$", "r", stdin);
		//freopen("CONOUT$", "w", stdout);
		//freopen("CONOUT$", "w", stderr);
	}

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

	RECT windowRect = {
		0L, 
		0L, 
		static_cast<long>(m_width), 
		static_cast<long>(m_height)
	};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Now create the window from the previously registered "DX11WindowClass" class 
	m_hwnd = CreateWindowA(
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


	if (!m_hwnd)
		return -1;

	ShowWindow(m_hwnd, nCmdShow);
	
	MSG msg = { 0 };

	Initialize();

	MainApplicationLoop(msg);

	// Release resources
	Shutdown();

	return static_cast<int>(msg.wParam);
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::MainApplicationLoop(MSG & msg) {
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			// Translates input virtual key messages to ASCII char key messages.
			TranslateMessage(&msg);

			// Dispatch message to our registered Window Procedure Callback function.
			DispatchMessage(&msg);
		}
		else {
			float dt = 0.0f;
			//-- Call overridden child-class methods:
			Update(dt);
			Render();
		}
	}
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::Initialize() {
	//-- Get the dimensions of the client window:
	RECT dimensions;
	GetClientRect(m_hwnd, &dimensions);

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
	swapChainDesc.OutputWindow = m_hwnd;
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
			&m_swapChain,
			&m_d3dDevice,
			&m_featureLevel,
			&m_d3dContext
		);

		if (SUCCEEDED(result)) {
			m_driverType = driverTypes[driverIndex];
			break;
		}
	}
	if (FAILED(result)) {
		DEBUG_MSG(L"Failed to create the Direct3D device.");
	}

	ID3D11Texture2D * backBufferTexture = nullptr;

	CHECK_DX_ERROR(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferTexture))
	);

	CHECK_DX_ERROR(
		m_d3dDevice->CreateRenderTargetView(backBufferTexture, nullptr, &m_backBufferTarget);
	);

	if (backBufferTexture) backBufferTexture->Release();

	m_d3dContext->OMSetRenderTargets(1, m_backBufferTarget.GetAddressOf(), nullptr);
	assert(m_backBufferTarget);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_d3dContext->RSSetViewports(1, &viewport);

	LoadContent();
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::Shutdown()
{
	UnloadContent();
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
