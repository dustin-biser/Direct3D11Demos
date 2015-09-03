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
		std::wstring name
	);

	virtual ~Dx11DemoBase();

	int Run (
		HINSTANCE hInstance,
		int nCmdShow
	);

	void Initialize();

	static LRESULT CALLBACK WindowProc (
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

	void Shutdown();

	virtual bool LoadContent();
	virtual void UnloadContent();
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	//-- Viewport dimensions:
	uint m_width;
	uint m_height;

	HWND m_hwnd;

	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11DeviceContext> m_d3dContext;
	ComPtr<ID3D11RenderTargetView> m_backBufferTarget; 

private:
	void MainApplicationLoop(MSG & msg);
};
