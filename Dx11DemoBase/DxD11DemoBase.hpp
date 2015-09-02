#pragma once

#include <d3d11.h>

class DxD11DemoBase {
public:
	DxD11DemoBase();

	virtual ~DxD11DemoBase();

	bool Initialize(
		HINSTANCE hInstance,
		HWND hwnd
	);

	void Shutdown();

	virtual bool LoadContent();
	virtual void UnloadContent();
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	HINSTANCE hInstance;
	HWND hwnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device * d3dDevice;
	ID3D11DeviceContext * d3dContext;
	IDXGISwapChain * swapChain;
	ID3D11RenderTargetView * backBufferTarget; 
};
