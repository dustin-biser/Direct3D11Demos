#ifdef _WIN32
	// Allow use of freopen() without compilation warnings/errors.
	// For use with customly allocated console window.
	#define _CRT_SECURE_NO_DEPRECATE
	#include <cstdio>
#endif

#include "Dx11DemoBase.hpp"
#include "NumericalTypes.hpp"
#include "Utils/CheckDxError.hpp"
#include "Utils/DebugMessage.hpp"

#include <vector>
#include <iostream>
using std::cerr;
using std::endl;
#include <cassert>
#include <chrono>
using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
#include <thread>

#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


// Forward Declaration:
static void centerWindow(HWND hWindow);


//---------------------------------------------------------------------------------------
Dx11DemoBase::Dx11DemoBase (
	uint width, 
	uint height,
	std::string windowTitle,
	float desiredFramesPerSecond
) : 
	  m_width(width),
	  m_height(height),
	  m_windowTitle(windowTitle),
	  m_desiredFramesPerSecond(desiredFramesPerSecond),
	  m_driverType(D3D_DRIVER_TYPE_NULL),
	  m_featureLevel(D3D_FEATURE_LEVEL_11_0)
{

}

//---------------------------------------------------------------------------------------
Dx11DemoBase::~Dx11DemoBase() 
{
	
}

//---------------------------------------------------------------------------------------
int Dx11DemoBase::run (
	HINSTANCE hInstance,
	int nCmdShow
) {
	//-- Use the following code to open a new console window and redirect stdout to it:
	{
		// Open a new console window
		AllocConsole();

		//-- Associate std input/output with newly opened console window:
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
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
		m_windowTitle.c_str(),
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

	centerWindow(m_hwnd);
	ShowWindow(m_hwnd, nCmdShow);
	
	initBase();

	// Begin the main message processing and rendering loop.
	MSG msg = { 0 };
	MainApplicationLoop(msg);

	// Release resources
	shutdown();

	return static_cast<int>(msg.wParam);
}

//---------------------------------------------------------------------------------------
static void centerWindow(HWND hWindow) {
	RECT windowRect;
	GetWindowRect(hWindow, &windowRect);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int topLeftXPos = (screenWidth - windowWidth) * 0.5f;
	int topLeftYPos = (screenHeight - windowHeight) * 0.5f;

	MoveWindow(hWindow, topLeftXPos, topLeftYPos, windowWidth, windowHeight, FALSE);
}

//---------------------------------------------------------------------------------------
void frameLimiter (
		float desiredFramesPerSecond,
		const steady_clock::time_point & frameStartTime
) {
	// Convert to milliseconds per frame
    uint desiredMsPerFrame = uint(1000.0f / desiredFramesPerSecond);

    steady_clock::time_point t2 (steady_clock::now());
    milliseconds elapsedTime (duration_cast<milliseconds>(t2 - frameStartTime));

	milliseconds d (desiredMsPerFrame);
    if(elapsedTime < milliseconds(desiredMsPerFrame)) {
		// TODO Dustin - Build error on this line:
        std::this_thread::sleep_for(d - elapsedTime);
    }
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::MainApplicationLoop(MSG & msg) {

	steady_clock::time_point frameStartTime;

	while (msg.message != WM_QUIT) {
		frameStartTime = steady_clock::now();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			// Translates input virtual key messages to ASCII char key messages.
			TranslateMessage(&msg);

			// Dispatch message to our registered Window Procedure Callback function.
			DispatchMessage(&msg);
		}
		else {
			float dt = 0.0f;
			//-- Call overridden derived-class methods:
			appLogic(dt);
			render();
		}

		frameLimiter(m_desiredFramesPerSecond, frameStartTime);
	}
}

//---------------------------------------------------------------------------------------
void Dx11DemoBase::initBase() {
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

	if (!DirectX::XMVerifyCPUSupport()) {
		DEBUG_MSG(L"Current architecture does not support DirectXMath.");
	}

	// Initialize derived class
	init();
}


//---------------------------------------------------------------------------------------
void Dx11DemoBase::init()
{
	// Override with demo specific content loading.
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
