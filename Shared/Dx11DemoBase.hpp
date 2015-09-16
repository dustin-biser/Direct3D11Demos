#pragma once

#include "NumericalTypes.hpp"

#include <string>

#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

class Dx11DemoBase {
public:
	Dx11DemoBase (
		uint width,
		uint height,
		std::string windowTitle,
		float desiredFramesPerSecond = 60.0f
	);

	virtual ~Dx11DemoBase();

	int run (
		HINSTANCE hInstance,
		int nCmdShow
	);

protected:
	void initBase();

	static LRESULT CALLBACK WindowProc (
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	virtual void init();
	virtual void appLogic(float dt) = 0;
	virtual void render() = 0;
	virtual void shutdown() = 0;

	//-- Viewport dimensions:
	uint m_width;
	uint m_height;
	std::string m_windowTitle;

	HWND m_hwnd;

	float m_desiredFramesPerSecond;

	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11DeviceContext> m_d3dContext;
	ComPtr<ID3D11RenderTargetView> m_backBufferTarget; 

private:
	void MainApplicationLoop(MSG & msg);
};
