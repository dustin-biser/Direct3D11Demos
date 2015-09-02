#pragma once

#include "NumericalTypes.hpp"

#include <string>

#include <d3d11.h>


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


	// Calls UnloadContent() and releases DirectX COM resources.
	void Shutdown();

	virtual bool LoadContent();
	virtual void UnloadContent();
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	//-- Viewpoert dimensions:
	uint width;
	uint height;

	HWND hwnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device * d3dDevice;
	ID3D11DeviceContext * d3dContext;
	IDXGISwapChain * swapChain;
	ID3D11RenderTargetView * backBufferTarget; 

private:
	void MainApplicationLoop(MSG & msg);
};
